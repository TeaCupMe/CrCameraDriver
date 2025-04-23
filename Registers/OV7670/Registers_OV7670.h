#ifndef REGISTERS_OV7670_H_
#define REGISTERS_OV7670_H_
//#include "Arduino.h"
#define CAMERA_ADDRESS 0x21


#define GAIN 0x00 // [7:0] Gain control 8 LSBs
#define BLUE 0x01 // [7:0] Blue gain control
#define RED 0x02 // [7:0] Red gain control

/* VREF
- [7:6] - Gain Control 2 MSBs (8 LSBs at GAIN[7:0])
- [5:4] - Reserved
- [3:2] - VREF end 2 LSBs (8 MSBs at VSTOP[7:0])
- [1:0] - VREF start 2 LSBs (8 MSBs at VSTRT[7:0]) 
 */
#define VREF 0x03

/* COM1
- [7] - Reserved
- [6] - Enable CCIR656
- [5:2] - Reserved
- [1:0] - AEC 2 LSBs (14 MSBs in AECHH[5:0] and AECH[7:0])
 */
#define COM1 0x04

/* AECHH
- [7:6] - Reserved
- [5:0] - AEC[15:10] (10 LSBs in AECH[7:0] and COM1[7:6]) 
 */
#define AECHH 0x07

#define BAVE 0x05 // U/B Average level
#define GbAVE 0x06 // Y/Gb Average Level
#define RAVE 0x08 // V/R Average Level

/* COM2
- [7:5] - Reserved
- [4] - Soft sleep mode
- [3:2] - Reserved
- [1:0] - Output drive capability
- - 00: 1x
- - 01: 2x
- - 10: 3x
- - 11: 4x
 */
#define COM2 0x09

/* COM3
- [7] - Reserved
- [6] - swap MSB and LSB in data output
- [5] - Tri-state output clock during power-down period
- - 0: enable
- - 1: disable
- [4] - Tri-state output data during power-down period
- - 0: enable
- - 1: disable
- [3] - Scale enable
- - 0: Disable
- - 1: Enable - if COM7[5:3] is set, COM14[3] must be 1
- [2] - DCW enable 
- - 0: Disable
- - 1: Enable - if COM7[5:3] is set, COM14[3] must be 1
- [1:0] - Reserved
 */
#define COM3 0x0C

/* COM4
- [7:6] - Reserved
- [5:4] - Average option (must match COM17[7:6]) 
- - 00: Full window
- - 01: 1/2 window
- - 10: 1/4 window
- - 11: 1/4 window
- [3:0] - Reserver
 */
#define COM4 0x0D

/* COM5
- [7:0] - Reserved
 */
#define COM5 0x0E

/* COM6 default: 0x43
- [7] Output at optical black line
- - 0: disable HREF at optical black
- - 1: enable HREF at optical black
- [6:2] - Reserved
- [1] Reset all timing when format changes
- - 0: No reset
- - 1: Reset
- [0] - Reserved

Note: COM6[3] has something to do with biases - BBIAS,GbBIAS,RBIAS
 */
#define COM6 0x0F

/* AECH default: 0x40
- [7:0] - AEC[9:2] (2 LSBs at COM1[7:6] and 6 MMSBs at AECHH[5:0])
 */
#define AECH 0x10

/* CLKRC default: 0x80
- [7] - Reserved
- [6] - Use external clock directly(no prescaler)
- [5:0] - Internal clock prescaler
- - F_int = F_ext/(CLKRC[5:0] + 1)
- - min = 62.5 kHz (prescaler = 127, F_ext = 8 MHz)
 */
#define CLKRC 0x11

/* COM7
- [7] - SCCB Register reset
- - 0: No change
- - 1: Reset all registers to default calues
- [6] - Reserved
- [5] - Output format - CIF selection
- [4] - Output format - QVGA selection
- [3] - Output format - QCIF selection
- [2] - Output format - RGB selection (see below)
- [1] - Color bar enable
- - 0: disable
- - 1: enable
- [0] - Output format - RAW RGB (see below)

                    |   COM7[2]   |   COM7[0]   |
        YUV         |      0      |      0      |
        RGB         |      1      |      0      |
     Bayer RGB      |      0      |      1      |
Processed Bayer RGB |      1      |      1      |

 */
#define COM7 0x12

/* COM8 default: 0x8F
- [7] - enable fast AGC/AEC algorithm
- [6] - AEC step size limit
- - 0: step size is limited to vertical blank
- - 1: unlimited step size
- [5] - Banding filter on/off (BD50ST or BD60ST must be nonzero)
- - 0: Off
- - 1: On
- [4:3] - Reserved
- [2] - AGC Enable
- [1] - AWB Enable
- [0] - AEC Enable
 */
#define COM8 0x13

/* COM9 default: 0x4a
- [7] - Reserved
- [6:4] - Auto gain Ceiling - max AGC
- - 000: 2x
- - 001: 4x
- - 010: 8x
- - 011: 16x
- - 100: 32x
- - 101: 64x
- - 110: 128x
- - 111: Not allowed
- [3:1] - Reserved
- [0] - Freeze AGC/AEC
*/
#define COM9 0x14

/* COM10 
- [7] - Reserved
- [6] - Use HSYNC instead of HREF
- [5] - PCLK mode
- - 0: PCLK always running
- - 1: PCLK disabled during horizontal breaks
- [4] - PCLK reverse
- [3] - HREF reverse
- [2] - VSYNC option
- - 0: VSYNC changes on falling edge of PCLK
- - 1: VSYNC changes on rising edge of PCLK
- [1] - VSYNC negative
- [0] - HSYNC negative
 */
#define COM10 0x15

/* HSTART default: 0x11 (136)
- [7:0] - Horizontal frame start
 */
#define HSTART 0x17

/* HSTOP default: 0x61 (776)
- [7:0] - Horizontal frame end
 */
#define HSTOP 0x18

/* VSTRT default: 0x03 (12)
- [7:0] - Vertical frame start
 */
#define VSTRT 0x19

/* VSTOP default: 0x7B (492)
- [7:0] - Vertical frame end
 */
#define VSTOP 0x1A

/* PSHFT 
- [7:0] - Pixel delay in pixel units
- - 00: no delay
- - FF: 256 pixel delay
 */
#define PSHFT 0x1B

/* MVFP default: 0x01
- [7:6] - Reserved
- [5] - Mirror
- - 0: Normal
- - 1: Mirror
- [4] - Vertical flip
- - 0: Normal
- - 1: Vertically flipped image
- [3] - Reserved
- [2] - Black sun enable
- [1:0] - Reserved
 */
#define MVFP 0x1E

/* ADCCTR0 default: 0x04
- [7:4] - Reserved
- [3] - ADC range
- - 0: 1x range
- - 1: 1.5x range
- [2:0] - ARC reference adjustments
- - 000 - 0.8x
- - 100 - 1x
- - 111 - 1.2x
 */
#define ADCCTR0 0x20

#define AEW 0x24 // AGC/AEC stable operating region upper limit. Default: 0x75
#define AEB 0x25 // AGC/AEC stable operating region lower limit. Default: 0x63

/* VPT default: 0xD4
- [7:4] - High nibble of upper limit of fast mode control zone
- [0:3] - High nibble of lower limit of fast mode control zone
 */
#define VPT 0x26

/* BBIAS default: 0x80
- [7] - Bias adjustment sign
- - 0: add bias
- - 1: subtract bias
- [6:0] - Bias value of 10-bit range(???)
*/
#define BBIAS 0x27

/* GbBIAS default: 0x80
- [7] - Bias adjustment sign
- - 0: add bias
- - 1: subtract bias
- [6:0] - Bias value of 10-bit range(???)
*/
#define GbBIAS 0x28

/* EXHCH
- [7:4] - 4 MSB for dummy pixel insert in horizontal direction
- [3:2] - HSYNC falling edge delay 2 MSB
- [1:0] - HSYNC rising edge delay 2 MSB
 */
#define EXHCH 0x2A

/* EXHCL
- [7:0] - 8 LSB for dummy pixel insert in horizontal direction
 */
#define EXHCL 0x2B

/* RBIAS default: 0x80
- [7] - Bias adjustment sign
- - 0: add bias
- - 1: subtract bias
- [6:0] - Bias value of 10-bit range(???)
 */
#define RBIAS 0x2C

#define ADVFL // LSB of Insert Dummy Lines in Vertical Direction
#define ADVFH // MSB of Insert Dummy Lines in Vertical Direction

#define YAVE // Y/G Average Level

/* HSYST default: 0x08
- [7:0] - HSYNC Rising edge Delay 8 LSBs
 */
#define HSYST 0x30

/* HSYEN default: 0x08
- [7:0] - HSYNC Falling edge Delay 8 LSBs
 */
#define HSYEN 0x30

/* HREF default: 0x80
- [7:6] - HREF edge offset to data output
- [5:3] - HREF end 3 LSBs (high 8 MSBs at HSTOP)
- [2:0] - HREF start 3 LSBs (high 8 MSBs at HSTART)
 */
#define REG_HREF 0x32

/* TSLB default: 0x0D
- [7:6] - Reserved
- [5] - Negative Image enable
- - 0: Normal image
- - 1: Negative image
- [4] - UV output value
- - 0: Use normal UV output
- - 1: Use fixed UV value set in MANU and MANV
- [3] - Output sequence (use with COM13[0])
      TSLB[3], COM13[0]
- - 00: Y U Y V
- - 01: Y V Y U
- - 10: U Y V Y
- - 11: V Y U Y
- [2:1] - Reserved
- [0] - Auto output window
- - 0: Sensor DOES NOT automatically set window after 
         resolution change. The companion backend processor
         can adjust the output window immediately after 
         changing the resolution
- - 1: Sensor automatically sets output window when
         resolution changes. After resolution changes,
         the companion backend processor must adjust the
         output window after the next VSYNC pulse.
 */
#define TSLB 0x3A

/* COM11
- [7] - Night mode
- - 0: disable
- - 1: enable - frame rate is reduced
- [6:5] - Minimum frame rate of night mode
- - 00: Same as normal mode
- - 01: 1/2 of normal mode
- - 10: 1/4 of normal mode
- - 11: 1/8 of normal mode
- [4] - D56_Auto
- - 0: Disable 50/60 HZ auto detection
- - 1: Enable 50/60 HZ auto detection
- [3] - Banding filter value select (when COM11[4] is set to 0)
- - 0: BD60ST selected
- - 1: BD50ST selected
- [2] - Reserved
- [1] - Exposure timing can be less, than limit of 
         banding filter when light is too strong
-  [0] Reserved
 */
#define COM11 0x3B

/* COM12 default: 0x68
- [7] - HREF option
- - 0: No HREF when VSYNC is low
- - 1: Always has HREF
- [6:0] - Reserved
 */
#define COM12 0x3C

/* COM13
- [7] - Gamma enable
- [6] - UV saturation level - UV auto adjustment. Result in SATCTR[3:0]
- [5:1] - Reserved
- [0] - UV swap (Use with TSLB[3])
      TSLB[3], COM13[0]
- - 00: Y U Y V
- - 01: Y V Y U
- - 10: U Y V Y
- - 11: V Y U Y
 */
#define COM13 0x3d

/* COM14
- [7:5] - Reserved
- [4] - DCW and scaling PCLK enable
- - 0: Normal PCLK
- - 1: DCW and scaling PCLK, controlled by register COM14[2:0] and SCALING_PCLK_DIV[3:0]
- [3] - Manual Scaling enable
- - 0: Scaling parameter cannot be adjusted manually
- - 1: Scaling parameter can be adjusted manually
- [2:0] - PCLK divider (when COM14[4] = 1)
- - 000: Divided by 1
- - 001: Divided by 2
- - 010: Divided by 4
- - 011: Divided by 8
- - 100: Divided by 16
- - 101~111: Not allowed
 */
#define COM14 0x3E

/* EDGE
- [7:5] - Reserved
- [4:0] - Edge enhancement factor
 */
#define EDGE 0x3F

/* COM15 default: 0xC0
- [7:6] - Data output range
- - 0x: Output range [10] to [F0]
- - 10: Output range [01] to [FE]
- - 11: Output range [00] to [FF]
- [5:4] - RGB 555/565 option (COM7[2]=1 and COM7[0]=0 required)
- [3:0] - Reserved
 */
#define COM15 0x40

/* COM16 default: 0x08
- [7:6] - Reserved
- [5] - Enable edge enchancement threshold auto-adjustment for YUV output
         Result saved in EDGE[4:0], range controlled by REG75[4:0] and REG76[4:0]
- - 0: Disable
- - 1: Enable
- [4] - Denoise threshold auto-adjustment 
         Result saved in DNSTH, range controlled by REG77[7:0]
- - 0: Disable
- - 1: Enable
- [3] - AWB gain enable
- [2] - Reserved
- [1] - Color matrix coefficient double option
- - 0: Original matrix
- - 1: Double of original matrix
- [0] - Reserved
 */
#define COM16 0x41

/* COM17
- [7:6] - AEC window, must match COM4[5:4]
 - - 00: Full window
- - 01: 1/2 window
- - 10: 1/4 window
- - 11: 1/4 window
- [5:4] - Reserved
- [3] - DSP color bar enable
- - 0: Disable
- - 1: Enable
- [2:0] - Reserved
 */
#define COM17 0x42

#define DNSTH 0x4B // De-noise Strength

/* Matrix coefficient 1 - 6 */
#define MTX1 0x4F // + [0-5]

#define BRIGHT 0x55 // Brightness control
#define CONTRAS 0x56 // Contrast control
#define CONTRAS_CENTER // Contrast center

/* MTXS default: 0x1E
- [7] - Auto contrast center enable
- - 0: Disable, center is set by CONTRAS_CENTER
- - 1: Enable, CONTRAS_CENTER is updated automatically
 */
#define MTXS 0x58

/* GFIX
- [7:6] - Fix gain for Gr channel
- - 00: 1x
- - 01: 1.25x
- - 10: 1.5x
- - 11: 1.75x
- [5:4] - Fix gain for Gb channel
- - 00: 1x
- - 01: 1.25x
- - 10: 1.5x
- - 11: 1.75x
- [3:2] - Fix gain for R channel
- - 00: 1x
- - 01: 1.25x
- - 10: 1.5x
- - 11: 1.75x
- [1:0] - Fix gain for B channel
- - 00: 1x
- - 01: 1.25x
- - 10: 1.5x
- - 11: 1.75x
 */
#define GFIX 0x69

/* DBVL default: 0x0A
- [7:6] - PLL control
- - 00: bypass
- - 01: F_ext x4
- - 10: F_ext x6
- - 11: F_ext x8
- [5] - Reserved
- [4] - Regulator control
- - 0: Enable internal regulator
- - 1: Disable internal regulator
- [3:0] - Reserved
 */
#define DBVL 0x6B

/* SCALING_XSC default: 3A
- [7] - Test_pattern[0] - works with test_pattern[1] (SCALING_YSC[7])
- - SCALING_XSC[7], SCALING_YSC[7]
- - 00: No test output
- - 01: Shifting 1
- - 10: 8-bar color bar
- - 11: Fade to gray color bar
- [6:0] - Horizontal scale factor
 */
#define SCALING_XSC 0x70

/* SCALING_YSC default: 35
- [7] - Test_pattern[1] - works with test_pattern[0] (SCALING_XSC[7])
- - SCALING_XSC[7], SCALING_YSC[7]
- - 00: No test output
- - 01: Shifting 1
- - 10: 8-bar color bar
- - 11: Fade to gray color bar
- [6:0] - Horizontal scale factor
 */
#define SCALING_YSC 0x71

/* SCALING_DCWCTR
- [7] - Vertical average calculation option
- - 0: Vertical truncation
- - 1: Vertical rounding
- [6] - Vertical downsampling option
- - 0: Vertical truncation
- - 1: Vertical rounding
- [5:4] - Vertical downsampling rate
- - 00: No vertical downsampling
- - 01: Vertical downsampling by 2
- - 10: Vertical downsampling by 4
- - 11: Vertical downsampling by 8 
- [3] - Horizontal average calculation option
- - 0: Horizontal truncation
- - 1: Horizontal rounding
- [2] - Horizontal downsampling option
- - 0: Horizontal truncation
- - 1: Horizontal rounding
- [1:0] - Horizontal downsampling rate
- - 00: No Horizontal downsampling
- - 01: Horizontal downsampling by 2
- - 10: Horizontal downsampling by 4
- - 11: Horizontal downsampling by 8 
 */
#define SCALING_DCWCTR 0x72

/* REG74
- [7:5] - Reserved
- [4] - DG_Manu
- - 0: Digital gain control by VREF[7:6]
- - 1: Digital gain control by REG74[1:0]
- [3:2] - Reserved
- [1:0] - Digital gain manual control
- - 00: Bypass
- - 01: 1x
- - 10: 2x
- - 11: 4x
 */
#define REG74 0x74

/* REG75 default: 0x0F
- [7:5] - Reserved
- [4:0] - Edge enhancement lower limit
 */
#define REG75 0x75

/* REG76 default: 0x01
- [7] - Black pixel correction enable
- - 0: Disable
- - 1: Enable
- [6] - White pixel correction enable
- - 0: Disable
- - 1: Enable
- [5] - Reserved
- [4:0] - Edge enhancement higher limit
 */
#define REG76 0x76

/* Gamma curve input  */
#define GAM1 0x7B // + [0-14]

/* RGB444
- [7:2] - Reserved
- [1] - RGB444 enable, effective when COM15[4] is high
- - 0: Disable
- - 1: Enalble
- [0] - RGB444 word format
- - 0: xR GB
- - 1: RG Bx
 */
#define RGB444 0x8C

/* NT_CTRL
- [7:4] - Reserved
- [3] - Auto frame rateadjustment control
- - 0: Double exposure time
- - 1: Reduce Frame rate by half
- [2] - Reserved
- [1:0] - Auto frame rate adjustment switch point
- - 00: Insert dummy row at 2x gain
- - 01: Insert dummy row at 4x gain
- - 10: Insert dummy row at 8x gain 
 */

#endif /** REGISTERS_OV7670_H_ */
