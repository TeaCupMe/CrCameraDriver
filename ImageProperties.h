/*
 * Image.h
 *
 *  Created on: Jul 11, 2024
 *      Author: alesh
 */

#ifndef CRCAMERADRIVER_IMAGEPROPERTIES_H_
#define CRCAMERADRIVER_IMAGEPROPERTIES_H_

//#include "stm32f4xx_hal.h"
#define MAX_CHUNK_PAYLOAD 240U

typedef enum {
	EMPTY,
	RGB565,
	RGB555,
	RGB444,
	YUV,
	WB // TODO: Modified YUV???
} Colorspace;

typedef struct {
	uint16_t height, width;
	uint16_t vStart, hStart;
	Colorspace colorspace = EMPTY;
	uint16_t exposure;
	uint32_t length;
	uint16_t numberOfChunks;

} ImageProperties;

typedef struct {
	uint16_t chunkID, payloadLength;
	bool isLastChunk;
	uint8_t payload[240];
	uint8_t checksum;
} Chunk;

#endif /* CRCAMERADRIVER_IMAGEPROPERTIES_H_ */
