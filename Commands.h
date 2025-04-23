/*
 * Commands.h
 *
 *  Created on: Oct 10, 2024
 *      Author: alesh
 */

#ifndef CRCAMERADRIVER_COMMANDS_H_
#define CRCAMERADRIVER_COMMANDS_H_

#define TAKE_PICTURE_CMD 't' // Take picture

#define REQUEST_IMAGE_PROPERTIES 'p'
#define RESET_CHUNK_COUNTER_CMD 'r' // set Chunk counter to 0
#define REQUEST_NEXT_CHUNK_CMD 'n'
#define REQUEST_CHUNK_ID_CMD 'i'
#define DUMP_IMAGE_CMD 'f'

#define SET_SIZE_CMD 's'
#define SET_EXPOSURE_CMD 'e'
#define SET_COLORSPACE_CMD 0x22


#endif /* CRCAMERADRIVER_COMMANDS_H_ */
