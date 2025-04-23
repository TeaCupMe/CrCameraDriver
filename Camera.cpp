/*
 * Camera.cpp
 *
 *  Created on: Jul 11, 2024
 *      Author: alesh
 */

/*	Int to Bin macros */

#define BYTE_TO_BIN_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BIN(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0')

/* 	Int to Bin macros END */

#include "Camera.h"
#include <cstdio>
#include <stdint.h>
#include "main.h"
#include "usart.h"

void Camera::setDebugUARTHandler(UART_HandleTypeDef* _huart) {
	_debugUart = _huart;
}

void Camera::setI2CHandler(I2C_HandleTypeDef* _hi2c) {
	_i2c = _hi2c;
}

HAL_StatusTypeDef Camera::setImageProperties(ImageProperties* imgP) {
	cameraState = INITIALIZATION;
	if (imgP->hStart + imgP->height > 656) {
		sprintf(errorMessage,  "hstart+height>640");
//		errorMessage = "hstart+height>640";
		cameraState = ERROR_INITIALIZATION;
		return HAL_ERROR;
	}
	if (imgP->vStart + imgP->width > 488) {
		sprintf(errorMessage, "vstart+width>480");
		cameraState = ERROR_INITIALIZATION;
		return HAL_ERROR;
	}
	imageProperties = *imgP;
	//TODO: write to registers
	cameraState = INITIALIZED;
	return HAL_OK;
}

void Camera::setPixelBuffer(uint8_t* p) {
	pixels = p;
}



template <typename T>
void Camera::printDebugToUart(T data) {
#ifdef CAMERA_DEBUG
	//HAL_UART_Transmit(_debugUart, (uint8_t*)&data, sizeof(data), 100*sizeof(data));
#endif /* DEBUG */
}
template void Camera::printDebugToUart<int>(int);
template void Camera::printDebugToUart<bool>(bool data);



void Camera::printDebugToUart(char const* data) {
#ifdef CAMERA_DEBUG
	int Size = 0;
	while (data[Size] != '\0') Size++;
	HAL_UART_Transmit(_debugUart, (uint8_t*)data, Size, 100*Size);
#endif /* DEBUG */
}

void Camera::printDebugToUart(char* data) {
#ifdef CAMERA_DEBUG
	int Size = 0;
	while (data[Size] != '\0') Size++;
	HAL_UART_Transmit(_debugUart, (uint8_t*)data, Size, 100*Size);
#endif /* DEBUG */
}

void Camera::dumpError() {
	HAL_UART_Transmit(_debugUart, (uint8_t*)errorMessage, 30, 100);
}

HAL_StatusTypeDef Camera::init() {
	return HAL_OK; //TODO add usable checks
}

Camera::Camera(uint16_t width, uint16_t height) { // @suppress("Class members should be properly initialized")
	imageProperties = {height, width, 3, 3, Colorspace::RGB565};
	cameraState = NOT_INITIALIZED;
}

Camera::Camera(ImageProperties _imgProp) { // @suppress("Class members should be properly initialized")
	imageProperties = _imgProp;
	cameraState = NOT_INITIALIZED;
}

Camera::Camera(ImageProperties _imgProp, UART_HandleTypeDef* uart, I2C_HandleTypeDef* i2c) { // @suppress("Class members should be properly initialized")
	imageProperties = _imgProp;
	_uart = uart;
	_i2c = i2c;
	cameraState = NOT_INITIALIZED;
}


HAL_StatusTypeDef Camera::setRegister(uint8_t addr, uint8_t value, uint8_t retries, bool ensure){           // REVIEW

	if (addr>0xD0) {
		cameraState = ERROR_REGISTER;
		sprintf(errorMessage, "Err reg write: addr==0x%02X", addr);
#ifdef CAMERA_DEBUG
		printDebugToUart("[E]> Error on setRegister(): addr too large: ");
		printDebugToUart(errorMessage+21);
		printDebugToUart("\n");
#endif	/* CAMERA_DEBUG */
		return HAL_ERROR;
	}


#ifdef CAMERA_DEBUG
	char* string = new char[46];
	sprintf(string, "[I]> Sending 0x%02X(" BYTE_TO_BIN_PATTERN ") to register 0x%02X\n", value, BYTE_TO_BIN(value), addr); //TODO Count character count and get rid of
	printDebugToUart(string);
//	HAL_UART_Transmit(_debugUart, (uint8_t*)string, 100, 300);
	delete[] string;
#endif

	while (retries>0) {
		// Write Register
		if (HAL_I2C_Mem_Write(_i2c, CAMERA_ADDRESS<<1, addr, I2C_MEMADD_SIZE_8BIT, &(value), 1U, 10) != HAL_OK) {
			retries--;
			printDebugToUart("\t - [E]> Failed to write to register!\n");
			continue;
		}
		printDebugToUart("\t - [I]> Register written\n");

		if (ensure) {
			// Select register to read
			if (HAL_I2C_Master_Transmit(_i2c, CAMERA_ADDRESS<<1, &(addr), 1, 10) != HAL_OK) {
				retries--;
				printDebugToUart("\t - [E]> Failed to select register!\n");
				continue;
			}
			printDebugToUart("\t - [I]> Register selected to read\n");
			// Read Register value
			uint8_t result;
			if (HAL_I2C_Master_Receive(_i2c, CAMERA_ADDRESS<<1 | 0x01, &result, 1, 10) != HAL_OK) {
				retries--;
				printDebugToUart("\t - [E]> Failed to read from register!\n");
				continue;
			}
			printDebugToUart("\t - [I]> Register read!\n");
		}
		return HAL_OK;
	}
	return HAL_ERROR;
}

HAL_StatusTypeDef Camera::setRegister(RegisterData* reg, uint8_t retries, bool ensure) {
	return Camera::setRegister(reg->addr, reg->value, retries, ensure);

}

uint8_t Camera::getRegister(uint8_t addr) {
	HAL_I2C_Master_Transmit(_i2c, CAMERA_ADDRESS<<1, &(addr), 1, 10);
	uint8_t result = 0;
	HAL_I2C_Master_Receive(_i2c, CAMERA_ADDRESS<<1 | 0x01, &result, 1, 10);
	return result;
}

HAL_StatusTypeDef Camera::setPrescaler(uint8_t prescaler) {
	return setRegister(CLKRC, 0x80 | (prescaler & 0b111111));
}

HAL_StatusTypeDef Camera::setRegisters(RegisterData* regs) {
	while (true) {
		if (regs->addr==0xFF) {
			return HAL_OK;
		}
		else {
			HAL_StatusTypeDef res = setRegister(regs->addr, regs->value);
			if (res != HAL_OK) {
				return res;
			}
			regs++;
		}
	}
}
void Camera::copyMemory(uint8_t *source, uint8_t *dest, uint32_t length) {
	for (uint32_t i = 0; i<length; i++) {
		*(dest+i) = *(source+i);
	}

}

void Camera::takePicture() {
	resetChunkCounter();
	uint8_t* pixelPointer = pixels;
	uint8_t factor = imageProperties.colorspace == Colorspace::WB ? 1 : 0;
	uint16_t lineLengthBytes = imageProperties.width * (factor==1?1:2);
	uint16_t imageHeightBytes = imageProperties.height;
	waitForVSYNCRising();
	HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
#ifdef DEBUG
	printDebugToUart("Start of frame\n");
//	HAL_UART_DeInit(&huart4);

#endif
	for (int y = 0; y < imageProperties.vStart; y++ ) {
		waitForHREFRising();
	}
	for (int y = 0; y < imageHeightBytes; y++) {
		waitForHREFRising();
//		HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
//		HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
		for (int x = 0; x < imageProperties.hStart; x++ ) {
			while (PCLK);
			while (!PCLK);
			while (PCLK);
			while (!PCLK);
		}
		for (int x = 0; x < lineLengthBytes; x++) {
//			waitForPCLKRising2();
			while (PCLK);
			while (!PCLK);
			while (PCLK);
			while (!PCLK);

//			waitForPCLKRising();
//			HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
			*pixelPointer++ = PIXEL;
//			pixelPointer++;
//			HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
		}
//		HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
	}
//	HAL_UART_Init(&huart4);
	HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
	imageProperties.length = lineLengthBytes * imageHeightBytes;
	imageProperties.numberOfChunks = (imageProperties.length-1)/MAX_CHUNK_PAYLOAD + 1;
	imageProperties.exposure = getExposure();
#ifdef DEBUG
	printDebugToUart("image captured");
#endif
	HAL_UART_Transmit(_uart, MESSAGE_START, 3, 1);
	HAL_UART_Transmit(_uart, MESSAGE_END, 3, 10);

}

void Camera::takePictureWB() {
	resetChunkCounter();
	uint8_t* pixelPointer = pixels;
//	uint8_t factor = imageProperties.colorspace == Colorspace::WB ? 1 : 0;
	uint16_t lineLengthBytes = imageProperties.width;
	uint16_t imageHeightBytes = imageProperties.height;
	waitForVSYNCRising();
//	HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
#ifdef DEBUG
	printDebugToUart("Start of frame\n");
#endif
	for (int y = 0; y < imageHeightBytes; y++) {
//		waitForHREFRising();
		waitForHREFLow();
		waitForHREFHigh();
//		HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
//		HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
		for (int x = 0; x < lineLengthBytes; x++) {
//			waitForPCLKRising(factor);
			waitForPCLKHigh();
			waitForPCLKLow();
			waitForPCLKHigh();
//			cam.waitForPCLKRising();
//			HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
			*pixelPointer = (uint8_t) PIXEL;
			pixelPointer++;
//			HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
		}
//		HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
	}
//	HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
	imageProperties.length = lineLengthBytes * imageHeightBytes;
	imageProperties.numberOfChunks = (imageProperties.length-1)/MAX_CHUNK_PAYLOAD + 1;
#ifdef DEBUG
	printDebugToUart("image captured");
#endif

}
void Camera::resetChunkCounter() {
	currentChunk = 0;
}

void Camera::sendChunkById(uint16_t id) {
	Chunk chunk = {0};
	chunk.chunkID = id;
//	chunk.imageProperties = imageProperties;
	if ((id + 1) * MAX_CHUNK_PAYLOAD < imageProperties.length) {
		chunk.isLastChunk = false;
		chunk.payloadLength = MAX_CHUNK_PAYLOAD;
		copyMemory(&(pixels[id*MAX_CHUNK_PAYLOAD]), chunk.payload, 240);

	} else {
		chunk.payloadLength = imageProperties.length - id*MAX_CHUNK_PAYLOAD;
		chunk.isLastChunk = true;
		copyMemory(&(pixels[id*MAX_CHUNK_PAYLOAD]), chunk.payload, chunk.payloadLength);
	}
	startCommand();
	pulseDebug(10);
	HAL_UART_Transmit(_uart, (uint8_t*)&chunk, sizeof(chunk), 300);
	endCommand();
}

void Camera::sendNextChunk() {
	pulseDebug(30);
	sendChunkById(currentChunk);
	pulseDebug(30);
	currentChunk=(currentChunk+1)%imageProperties.numberOfChunks;

}

void Camera::sendImageProperties() {
	startCommand();
	pulseDebug(30);
	HAL_UART_Transmit(_uart, (uint8_t*)&imageProperties, sizeof(imageProperties), 100);
	pulseDebug(30);
	endCommand();

}

void Camera::startCommand() {
	HAL_UART_Transmit(_uart, MESSAGE_START, 3, 20);
//	HAL_Delay(5);
}

void Camera::endCommand() {
	HAL_UART_Transmit(_uart, MESSAGE_END, 3, 20);
}

void Camera::pulseDebug(int length) {
//	HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
//	HAL_Delay(length);
//	HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
}

HAL_StatusTypeDef Camera::setExposure(uint16_t exposure) { //TODO
	uint8_t aechh = (uint16_t) (getRegister(AECHH) & 0b00111111);
	uint8_t aech = (uint16_t) (getRegister(AECH) & 0b11111111);
	uint8_t aec = (uint16_t) (getRegister(COM1) & 0b00000011);

	aechh = (aechh & (~0b00111111)) | ((exposure >> 10) & 0b00111111);
	aech = (aech & (~0b11111111)) | ((exposure >> 2) & 0b11111111);
	aec = (aec & (~0b00000011)) | (exposure & 0b00000011);

	setRegister(AECHH, aechh);
	setRegister(AECH, aech);
	return setRegister(COM1, aec);

//	return HAL_StatusTypeDef::HAL_TIMEOUT;
}

uint16_t Camera::getExposure() {
	uint16_t aechh = (uint16_t) (getRegister(AECHH) & 0b00111111);
	uint16_t aech = (uint16_t) (getRegister(AECH) & 0b11111111);
	uint16_t aec = (uint16_t) (getRegister(COM1) & 0b00000011);

	return (aechh << 10) | (aech << 2) | (aec);

}

HAL_StatusTypeDef Camera::enableAutoExposure(bool enable) {
	return setRegister(COM8, enable?0b11001011:0b11001010);
}

