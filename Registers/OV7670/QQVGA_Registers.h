#ifndef QQVGA_REGISTERS_H_
#define QQVGA_REGISTERS_H_

#include "Registers.h"

#define QQVGA_VERTICAL_PADDING 2
#define QQVGA_VSTART 0
#define QQVGA_VSTOP (120 + QQVGA_VERTICAL_PADDING)

#define QQVGA_HSTART 182
#define QQVGA_HSTOP 46
// namespace OV7670
// {
    const RegisterData regsQQVGA[] = {
        {REG_VSTART, QQVGA_VSTART},
        {REG_VSTOP, QQVGA_VSTOP},
        {REG_VREF, 0},
        {REG_HSTART, QQVGA_HSTART >> 3},
        {REG_HSTOP, QQVGA_HSTOP >> 3},
        {REG_HREF, 0b00000000 | (QQVGA_HSTART & 0b111) | ((QQVGA_HSTOP & 0b111) << 3)},

        {REG_COM3, COM3_DCWEN},   // enable downsamp/crop/window
        {REG_COM14, 0x1a},        // divide by 4
        {SCALING_DCWCTR, 0x22},   // downsample by 4
        {SCALING_PCLK_DIV, 0xf2}, // divide by 4

        {0xff, 0xff} /* END MARKER */
    };
// } // namespace OV7670
#endif // QQVGA_REGISTERS_H_