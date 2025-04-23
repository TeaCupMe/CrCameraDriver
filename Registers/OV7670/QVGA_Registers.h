#ifndef QVGA_REGISTERS_H_
#define QVGA_REGISTERS_H_

#include "Registers.h"

#define QVGA_VERTICAL_PADDING 5
#define QVGA_VSTART 0
#define QVGA_VSTOP (240 + QVGA_VERTICAL_PADDING)

#define QVGA_HSTART 174
#define QVGA_HSTOP 34
// namespace OV7670
// {
    const RegisterData regsQVGA[] = {
        {REG_VSTART, QVGA_VSTART >> 1},
        {REG_VSTOP, QVGA_VSTOP >> 1},
        {REG_VREF, 0b00000000 | ((QVGA_VSTART & 0b1) << 1) | ((QVGA_VSTOP & 0b1) << 3)},
        {REG_HSTART, QVGA_HSTART >> 3},
        {REG_HSTOP, QVGA_HSTOP >> 3},
        {REG_HREF, 0b00000000 | (QVGA_HSTART & 0b111) | ((QVGA_HSTOP & 0b111) << 3)},

        {REG_COM3, COM3_DCWEN},   // enable downsamp/crop/window
        {REG_COM14, 0x19},        // divide by 2
        {SCALING_DCWCTR, 0x11},   // downsample by 2
        {SCALING_PCLK_DIV, 0xf1}, // divide by 2

        {0xff, 0xff} /* END MARKER */
    };
// } // namespace OV7670

#endif // QVGA_REGISTERS_H_