#ifndef REGISTERS_OV7725_H_
#define REGISTERS_OV7725_H_
#define CAMERA_ADDRESS 0x21


// Названия и адреса основных регистров и значения

#define GAIN 0x00 // [7:0] Контроль усиления
#define BLUE 0x01 // [7:0] Контроль усиления синего
#define RED 0x02 // [7:0] Контроль усиления красного
#define GREEN 0x03 // [7:0] Контроль усиления зелёного

#define AECH 0x08 // Старшие 8 бит авто-экспозиции(остальное в AEC)

/* COM2
- [7:5] - reserved
- [4] - soft sleep 
- [1:0] - output drive
- - 00: 1x
- - 01: 2x
- - 10: 3x
- - 11: 4x 
*/ 
#define COM2 0x09 

/* COM3
- [7] - Vertical flip
- [6] - Horizontal mirror
- [5] - swap B/R in RGB
- [4] - swap Y/UV in YUV
- [3] - swap MSB/LSB
- [2] - !tri-state output clk at power-down
- [1] - !tri-state output dta at power-down
- [0] - color bar enable
*/
#define COM3 0x0C

/* COM4
- [7-6] - PLL control
- - 00: bypass
- - 01: 4x
- - 10: 6x
- - 11: 8x
- [5:4] - AEC evaluate window - место работы АКЭ
- - 00: full
- - 01: 1/2
- - 10: 1/4
- - 11: lower 2/3
*/
#define COM4 0x0d

/* COM5
- [7] - Auto frame rate control on/off
- [6] - Auto frame rate control speed selection
- [5:4] - Auto frame rate max rate
- - 00: no reduction
- - 01: max reduction to 1/2 frame rate
- - 10: max reduction to 1/4 frame rate
- - 11: max reduction to 1/8 frame rate
- [3:2] - Auto frame rate active point control
- - 00: not allowed
- - 01: add frame when AGC reaches 4x
- - 10: add frame when AGC reaches 8x
- - 11: add frame when AGC reaches 16x
- [1] - reserver
- [0] - AEC max step
- - 1: has limit
- - 0: no limit
*/
#define COM5 0x0e

/* COM6 
- [7:1] - reserved
- [0] - auto window setting ON/OFF when format changes
*/
#define COM6 0x0f

#define AEC 0x10 // [7:0] - 8 low bits of exposure value

/* CLKRC
- [7] - reserved
- [6] - use ext clock directly
- [5:0] - clock prescaler F_o = F_i/([5:0]+1)/2
*/
#define CLKRC 0x11 

/* COM7
- [7] - SCCB Register reset
- - 0: no change
- - 1: reset all to default
- [6] - Select Resolution
- - 0: VGA
- - 1: QVGA 
- [5] - BT.656 on/off
- [4] - Sensor RAW
- [3:2] - RGB output format
- - 00: GBR4:2:2
- - 01: RGB565
- - 10: RGB555
- - 11: RGB444
- [1:0] - output format control
- - 00: YUV
- - 01: Processsed Bayer RAW
- - 10: RGB
- - 11: Bayer RAW
*/
#define COM7 0x12

/* COM8
- [7] - Enable fast AGC/AEC algo
- [6] - AEC step size limit
- - 0: step size limited to vert blank
- - 1: unlimited step size
- [5] - Banding filter ON/OFF
- [4] - Enable AEC below banding value
- [3] - Fine AEC on/off
- [2] - AGC enable
- [1] - AWB enable
- [0] - AEC enable
 */
#define COM8 0x13

/* COM9
- [7] - histogramm or average-based AEC/AGC
- [6:4] - AG Ceilng - max AGC value:
- - 000: 2x
- - 001: 4x
- - 010: 8x
- - 011: 16x
- - 100: 32x
- - 101: not allowed
- - 110: not allowed
- - 111: not allowed
- [3] - reserved
- [2] - Drop VSYNC of corrupt frame
- [1] - drop HREF of corrupt frame
- [0] - reserved
 */
#define COM9 0x14

/* COM10
- [7] - output negative
- [6] - change HREF to HSYNC
- [5] - PCLK output option:
- - 0: Free running PCLK
- - 1: PCLK off during line breaks
- [4] - PCLK reverse
- [3] - HREF reverse
- [2] - VSYNC option:
- - 0: VSYNC changes on FE of PCLK
- - 1: VSYNC changes on RE of PCLK
- [1] - VSYNC negative
- [0] - Output data range
- - 0: Full Range
- - 1: Data from [10] to [F0] (8MSBs) //REVIEW 
 */
#define COM10 0x15

/* HSTART 
- [7:0] - Horizontal frame start 8MSBs (2 LSBs at HREF[5:4])
 */
#define HSTART 0x17

/* HSIZE
- [7:0] - Horizontal sensor size 8MSBs (2 LSBs at HREF[1:0])
 */
#define HSIZE 0x18

/* VSTRT
- [7:0] - Vertical frame start 8MSBs (1 LSB at HREF[6])
*/
#define VSTRT 0x19

/* VSIZE
 - [7:0] - Vertical sensor size 8MSBs (1 LSB at HREF[2])
*/
#define VSIZE 0x1A

/* PSHFT
- [7:0] - Pixel delay select - delays timing of D[9:0] relative to HREF
- - 00: no delay
- - ...... : ..... pixel delay
- - FF: 256 pixel delay
*/
#define PSHFT 0x1B

/* COM11
- [7:2] - reserved
- [1] - single frame on/off
- [0] - single frame transfer trigger
*/
#define COM11 0x20

/* HOUTSIZE


*/
#define HOUTSIZE 0x29






// #include "QQVGA_Registers.h"
// #include "VGA_Registers.h"
// #include "QVGA_Registers.h"

#endif // REGISTERS_OV7725_H_

