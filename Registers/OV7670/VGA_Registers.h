// #error : VGA not implemented
#ifndef VGA_REGISTERS_H_
#define VGA_REGISTERS_H_

#include "Registers.h"

#define VGA_VERTICAL_PADDING 5
#define VGA_VSTART 0
#define VGA_VSTOP (240 + VGA_VERTICAL_PADDING)

#define VGA_HSTART 174
#define VGA_HSTOP 34

namespace OV7670
{

    RegisterData regsVGA[] = {
        {REG_VSTART, VGA_VSTART >> 1},
        {REG_VSTOP, VGA_VSTOP >> 1},
        {REG_VREF, 0b00000000 | ((VGA_VSTART & 0b1) << 1) | ((VGA_VSTOP & 0b1) << 3)},
        {REG_HSTART, VGA_HSTART >> 3},
        {REG_HSTOP, VGA_HSTOP >> 3},
        {REG_HREF, 0b00000000 | (VGA_HSTART & 0b111) | ((VGA_HSTOP & 0b111) << 3)},

        {REG_COM3, COM3_DCWEN},   // enable downsamp/crop/window
        {REG_COM14, 0x19},        // divide by 2
        {SCALING_DCWCTR, 0x11},   // downsample by 2
        {SCALING_PCLK_DIV, 0xf1}, // divide by 2

        {0xff, 0xff} /* END MARKER */
    };
} // namespace OV7670

namespace OV7725 {
    RegisterData regsVGA[] = {

    };
}
#endif // VGA_REGISTERS_H_