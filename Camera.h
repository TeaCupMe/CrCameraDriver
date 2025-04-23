/*
 * Camera.h
 *
 *  Created on: Jul 11, 2024
 *      Author: alesh
 */

#ifndef CRCAMERADRIVER_CAMERA_H_
#define CRCAMERADRIVER_CAMERA_H_

#include "main.h"

// Uncomment line below to enable profiling camera via UART4
//#define CAMERA_DEBUG
#define USE_OV7670




#define PIXEL ((GPIOC->IDR) & 0b11111111)

//#define PCLK (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2)/*==GPIO_PIN_SET*/) 		// Поменять для модуля
#define PCLK ((GPIOA->IDR) & 0b00000100)
//#define HREF (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3)/*==GPIO_PIN_SET*/)		// Поменять для модуля
#define HREF ((GPIOA->IDR) & 0b00001000)
//#define VSYNC (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4)/*==GPIO_PIN_SET*/)		// Поменять для модуля
#define VSYNC ((GPIOA->IDR) & 0b00010000)

#include <stdint.h>
#include "ImageProperties.h"
#include "Registers/Registers.h"
#include "CameraState.h"


class Camera
{
public:

	Camera(uint16_t width = 160, uint16_t height=120);
	Camera(ImageProperties imgProp);
	Camera(ImageProperties imgProp, UART_HandleTypeDef* uart, I2C_HandleTypeDef* i2c);

	CameraState cameraState = DEFAULT;
	ImageProperties imageProperties;
	uint8_t prescaler = 4; //private, accessible through setter/getter
	uint8_t exposure = 0;
	uint16_t currentChunk = 0;
	
	const uint8_t MESSAGE_START[3] = {0xFF, 0xFF, 0x00};
	const uint8_t MESSAGE_END[3] = {0x00, 0xFF, 0x00};

	bool autoExposure = true;


	/* Note:
	 * 		There are two UART handlers in this driver. It is possible to use
	 * 		the same UART interface twice, but some operations may be slower or
	 * 		less stable.
	 * */
	UART_HandleTypeDef* _debugUart = nullptr;   // Used primarily in debug purposes
	UART_HandleTypeDef* _uart = nullptr;		// Used to communicate to and configure driver

	I2C_HandleTypeDef* _i2c = nullptr;
	SPI_HandleTypeDef* _spi = nullptr;
	uint8_t* pixels;
	char errorMessage[30] = {0};

	void setI2CHandler(I2C_HandleTypeDef* _hi2c);
	void setDebugUARTHandler(UART_HandleTypeDef* _huart);

	HAL_StatusTypeDef setImageProperties(ImageProperties*);
	void setPixelBuffer(uint8_t* pixels);

	HAL_StatusTypeDef setExposure(uint16_t);
	uint16_t getExposure();
	HAL_StatusTypeDef enableAutoExposure(bool enable);
	HAL_StatusTypeDef setPrescaler(uint8_t);

	HAL_StatusTypeDef init();

	void takePicture();
	void takePictureWB();

	void sendImageProperties();
	void resetChunkCounter();
	void sendChunkById(uint16_t id);
	void sendNextChunk();
	void startCommand();
	void endCommand();
//private: // TODO: uncomment this line


	/**
	 * @brief Transmits an arbitrary object over UART.
	 * @note If CAMERA_DEBUG is not defined, does nothing and compiles empty.
	 * @attention In order for this method to work properly one should add template specification. i.e.:
	 * 		template void Camera::printDebugToUart<bool>(bool);
	 *
	 * @tparam T Arbitrary data type
	 * @param data T-typed object
	 */
	template <typename T>
	void printDebugToUart(T);

	/**
	 * @brief Transmits a message over UART.
	 * @note If CAMERA_DEBUG is not defined, does nothing and compiles empty
	 *
	 * @param data Pointer to const char array (aka string).
	 */
	void printDebugToUart(char const*);

	/**
	 * @brief Transmits a message over UART.
	 * @note If CAMERA_DEBUG is not defined, does nothing and compiles empty
	 *
	 * @param data Pointer to char array.
	 */
	void printDebugToUart(char*);

	/**
	 * @brief Pulses LED0 for a given amount of milliseconds
	 *
	 * @param length Pulse length in milliseconds
	 */
	void pulseDebug(int length = 10);

	/**
	 * @brief Dumps contents of errorMessage string to debug UART
	 *
	 */
	void dumpError();

	/**
	 * @brief Writes a value to camera register with specified number of retries on failure.
	 *
	 * @param reg Pointer to RegiserData structure with register address and desired value
	 * @param retries How many times the driver retries to write to register
	 * @param ensure If true, driver reads the written register after each operation and
	 * 					checks if written value is in register
	 * @return HAL-based status of operation
	 */
	HAL_StatusTypeDef setRegister(RegisterData* reg, uint8_t retries = 4, bool ensure = false); // REVIEW

	/**
	 * @brief Writes a value to camera register with specified number of retries on failure.
	 *
	 * @param addr Register address
	 * @param value Register value
	 * @param retries How many times the driver retries to write to register
	 * @param ensure If true, driver reads the written register after each operation and
	 * 					checks if written value is in register
	 * @return HAL-based status of operation
	 */
	HAL_StatusTypeDef setRegister(uint8_t addr, uint8_t value, uint8_t retries = 4, bool ensure = false);           // REVIEW

	/**
		 * @brief Writes bits, selected by mask, to specified address
		 *
		 * @param addr Register address
		 * @param value Register value
		 * @param mask Bitmask
		 * @param ensure If true, driver reads the written register after each operation and
		 * 					checks if written value is in register
		 * @return HAL-based status of operation
		 */
	HAL_StatusTypeDef setMaskedRegister(uint8_t addr, uint8_t value, uint8_t mask = 0b11111111, bool ensure = false);

	/**
	 * @brief Writes registers from specified pointer forward in memory, until register
	 * 					with addr==0xFF is found
	 * @param regs Pointer at the first RegisterData struct
	 * @return
	 */
	HAL_StatusTypeDef setRegisters(RegisterData* regs); // Add led for error handling

	void readRegister(RegisterData* reg, uint8_t retries = 4);                           // REVIEW
	uint8_t getRegister(RegisterData *regData); // REVIEW
	uint8_t getRegister(uint8_t addr);          // REVIEW
	uint8_t getMaskedRegister(uint8_t addr, uint8_t mask);
	
	

	inline void waitForPCLKRisingN(int skip);
	inline void waitForPCLKFallingN(int skip);
	inline void waitForPCLKRising2();
	inline void waitForPCLKFalling2();
	inline void waitForPCLKRising();
	inline void waitForPCLKFalling();
	inline void waitForPCLKHigh();
	inline void waitForPCLKLow();
	inline void waitForHREFRising();
	inline void waitForHREFFalling();
	inline void waitForHREFRisingN(int skip);
	inline void waitForHREFFallingN(int skip);
	inline void waitForHREFHigh();
	inline void waitForHREFLow();
	inline void waitForVSYNCRising();
	inline void waitForVSYNCFalling();
	inline void waitForVSYNCRisingN(int skip);
	inline void waitForVSYNCFallingN(int skip);
	inline void waitForVSYNCHigh();
	inline void waitForVSYNCLow();



	void copyMemory(uint8_t* source, uint8_t* dest, uint32_t length);
};


/* Inline Synchronization methods */
inline void Camera::waitForPCLKRising() {
	while (PCLK);
	while (!PCLK);
//	while ((((GPIO_TypeDef *) (((0x40000000UL) + 0x18020000UL) + 0x0000UL))->IDR) & 0b00000100);
//	while (!(((((GPIO_TypeDef *) (((0x40000000UL) + 0x18020000UL) + 0x0000UL))->IDR) & 0b00000100)));
}
inline void Camera::waitForPCLKFalling() {
	while (!PCLK);
	while (PCLK);

}

inline void Camera::waitForPCLKRising2() {
//	while (PCLK);
//	while (!PCLK);
//	while (PCLK);
//	while (!PCLK);
	while ((((GPIO_TypeDef *) (((0x40000000UL) + 0x18020000UL) + 0x0000UL))->IDR) & 0b00000100);
	while (!(((((GPIO_TypeDef *) (((0x40000000UL) + 0x18020000UL) + 0x0000UL))->IDR) & 0b00000100)));
	while ((((GPIO_TypeDef *) (((0x40000000UL) + 0x18020000UL) + 0x0000UL))->IDR) & 0b00000100);
	while (!(((((GPIO_TypeDef *) (((0x40000000UL) + 0x18020000UL) + 0x0000UL))->IDR) & 0b00000100)));
}
inline void Camera::waitForPCLKFalling2() {
	while (!PCLK);
	while (PCLK);
	while (!PCLK);
	while (PCLK);
}
inline void Camera::waitForPCLKRisingN(int skip) {
	do {
		skip--;
	while (PCLK);
	while (!PCLK);

	} while (skip>=0);
}
inline void Camera::waitForPCLKFallingN(int skip) {
	do {
		skip--;
	while (!PCLK);
	while (PCLK);

	} while (skip>=0);
}

inline void Camera::waitForHREFRising() {
	while (HREF);
	while (!HREF);
}
inline void Camera::waitForHREFFalling() {
	while (!HREF);
	while (HREF);
}
inline void Camera::waitForHREFRisingN(int skip) {
	do {
		skip--;
	while (HREF);
	while (!HREF);

	} while (skip>=0);
}
inline void Camera::waitForHREFFallingN(int skip) {
	do {
		skip--;
	while (!HREF);
	while (HREF);

	} while (skip>=0);
}
inline void Camera::waitForVSYNCRising() {
	while (VSYNC);
	while (!VSYNC);
}
inline void Camera::waitForVSYNCFalling(){
	while (!VSYNC);
	while (VSYNC);
}
inline void Camera::waitForVSYNCRisingN(int skip) {
	do {
		skip--;
	while (VSYNC);
	while (!VSYNC);

	} while (skip>=0);
}
inline void Camera::waitForVSYNCFallingN(int skip){
	do {
		skip--;
	while (!VSYNC);
	while (VSYNC);

	} while (skip>=0);
}

inline void Camera::waitForPCLKHigh() {
	while (!PCLK);
}
inline void Camera::waitForPCLKLow() {
	while (PCLK);
}
inline void Camera::waitForHREFHigh() {
	while (!HREF);
}
inline void Camera::waitForHREFLow() {
	while (HREF);
}
inline void Camera::waitForVSYNCHigh() {
	while (!VSYNC);
}


inline void Camera::waitForVSYNCLow() {
	while (VSYNC);
}



#endif /* CRCAMERADRIVER_CAMERA_H_ */
