#ifndef REGISTERS_H_
#define REGISTERS_H_

#if defined USE_OV7670 && defined USE_OV7725
#error Both USE_OV7670 and USE_OV7725 defined. Use only one of them
#elif !defined USE_OV7670 && !defined USE_OV7725
#error Neither USE_OV7670 nor USE_OV7725 are defined. Define one of them with #define USE_OV7xxx
#endif


#ifdef USE_OV7670
#include "./OV7670/Registers_OV7670.h"
#elif defined USE_OV7725
#include "./OV7725/Registers_OV7725.h"
#endif


#define PID 0x0A // Product ID MSB
#define VER 0x0B // Product ID LSB

typedef struct { // Удобный объект для хранения данных для записи в регистр камеры
  uint8_t addr;       // Адрес регистра
  uint8_t value;        // Значения для записи
} RegisterData;
#endif /** REGISTERS_H_ */
