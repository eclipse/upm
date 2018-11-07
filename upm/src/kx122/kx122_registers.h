/*
The MIT License (MIT)
Copyright (c) 2017 Kionix Inc.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef __KX122_REGISTERS_H__
#define __KX122_REGISTERS_H__
/* registers */
// x- hp filter output
#define KX122_XHP_L 0x00
#define KX122_XHP_H 0x01
// y- hp filter output
#define KX122_YHP_L 0x02
#define KX122_YHP_H 0x03
// z- hpfilteroutput
#define KX122_ZHP_L 0x04
#define KX122_ZHP_H 0x05
// output register x
#define KX122_XOUT_L 0x06
#define KX122_XOUT_H 0x07
// output register y
#define KX122_YOUT_L 0x08
#define KX122_YOUT_H 0x09
// output register z
#define KX122_ZOUT_L 0x0A
#define KX122_ZOUT_H 0x0B
// communication selftest
#define KX122_COTR 0x0C
// WHO_AM_I
#define KX122_WHO_AM_I 0x0F
// current sixfacet posititions
#define KX122_TSCP 0x10
// previous six facet positions
#define KX122_TSPP 0x11
// This register indicates the triggering axis when a tap/double tap interrupt occurs.
#define KX122_INS1 0x12
// This register tells witch function caused an interrupt.
#define KX122_INS2 0x13
// This register reports the axis and direction of detected motion.
#define KX122_INS3 0x14
// This register reports the status of the interrupt.
#define KX122_STATUS_REG 0x15
#define KX122_INT_REL 0x17
// Read/write control register that controls the main feature set.
#define KX122_CNTL1 0x18
// 2' control register
#define KX122_CNTL2 0x19
// 3' controlregister
#define KX122_CNTL3 0x1A
// This register is responsible for configuring ODR (output data rate) and filter settings
#define KX122_ODCNTL 0x1B
// This register controls the settings for the physical interrupt pin INT1
#define KX122_INC1 0x1C
// This register controls which axis and direction of detected motion can cause an interrupt.
#define KX122_INC2 0x1D
// This register controls which axis and direction of tap/double tap can cause an interrup
#define KX122_INC3 0x1E
// This register controls routing of an interrupt reporting to physical interrupt pin INT1
#define KX122_INC4 0x1F
// This register controls the settings for the physical interrupt pin INT2.
#define KX122_INC5 0x20
// This register controls routing of interrupt reporting to physical interrupt pin INT2
#define KX122_INC6 0x21
#define KX122_TILT_TIMER 0x22
#define KX122_WUFC 0x23
// This register is responsible for enableing/disabling reporting of Tap/Double Tap.
#define KX122_TDTRC 0x24
#define KX122_TDTC 0x25
#define KX122_TTH 0x26
#define KX122_TTL 0x27
#define KX122_FTD 0x28
#define KX122_STD 0x29
#define KX122_TLT 0x2A
#define KX122_TWS 0x2B
#define KX122_FFTH 0x2C
#define KX122_FFC 0x2D
// Free Fall Control: This register contains the counter setting of the Free fall detection.
#define KX122_FFCNTL 0x2E
#define KX122_ATH 0x30
#define KX122_TILT_ANGLE_LL 0x32
#define KX122_TILT_ANGLE_HL 0x33
// This register sets the Hysteresis that is placed in between the Screen Rotation states
#define KX122_HYST_SET 0x34
// Low Power Control sets the number of samples of accelerometer output to be average
#define KX122_LP_CNTL 0x35
// Read/write control register that controls the buffer sample threshold
#define KX122_BUF_CNTL1 0x3A
// Read/write control register that controls sample buffer operation
#define KX122_BUF_CNTL2 0x3B
// This register reports the status of the sample buffer
#define KX122_BUF_STATUS_1 0x3C
// This register reports the status of the sample buffer trigger function
#define KX122_BUF_STATUS_2 0x3D
#define KX122_BUF_CLEAR 0x3E
#define KX122_BUF_READ 0x3F
// When 0xCA is written to this register, the MEMS self-test function is enabled. Electrostatic-actuation of the accelerometer, results in a DC shift of the X, Y and Z axis outputs. Writing 0x00 to this register will return the accelerometer to normal operation
#define KX122_SELF_TEST 0x60
// WHO_AM_I
#define KX112_WHO_AM_I 0x0F
// WHO_AM_I
#define KX123_WHO_AM_I 0x0F
// WHO_AM_I
#define KX124_WHO_AM_I 0x0F
/* registers bits */
// before set
#define KX122_COTR_DCSTR_BEFORE (0x55 << 0)
// after set
#define KX122_COTR_DCSTR_AFTER (0xAA << 0)
// WHO_AM_I -value for KX122
#define KX122_WHO_AM_I_WIA_ID (0x1B << 0)
// x-left
#define KX122_TSCP_LE (0x01 << 5)
// x+right
#define KX122_TSCP_RI (0x01 << 4)
// y-down
#define KX122_TSCP_DO (0x01 << 3)
// y+up
#define KX122_TSCP_UP (0x01 << 2)
// z-facedown
#define KX122_TSCP_FD (0x01 << 1)
// z+faceup
#define KX122_TSCP_FU (0x01 << 0)
// x-left
#define KX122_TSPP_LE (0x01 << 5)
// x+right
#define KX122_TSPP_RI (0x01 << 4)
// y-down
#define KX122_TSPP_DO (0x01 << 3)
// y+up
#define KX122_TSPP_UP (0x01 << 2)
// z-facedown
#define KX122_TSPP_FD (0x01 << 1)
// z+faceup
#define KX122_TSPP_FU (0x01 << 0)
// x-
#define KX122_INS1_TLE (0x01 << 5)
// x+
#define KX122_INS1_TRI (0x01 << 4)
// y-
#define KX122_INS1_TDO (0x01 << 3)
// y+
#define KX122_INS1_TUP (0x01 << 2)
// z-
#define KX122_INS1_TFD (0x01 << 1)
// z+
#define KX122_INS1_TFU (0x01 << 0)
// Free fall. This bit is cleared when the interrupt latch release register (INL) is read..
#define KX122_INS2_FFS (0x01 << 7)
// indicates buffer full interrupt. Automatically cleared when buffer is read.
#define KX122_INS2_BFI (0x01 << 6)
// Watermark interrupt, bit is set to one when FIFO has filled up to the value stored in the sample bits.This bit is automatically cleared when FIFO/FILO is read and the content returns to a value below the value stored in the sample bits.
#define KX122_INS2_WMI (0x01 << 5)
// indicates that new acceleration data (0x06h to 0x0Bh) is available. This bit is cleared when acceleration data is read or the interrupt release register INT_REL is read.
#define KX122_INS2_DRDY (0x01 << 4)
// no tap
#define KX122_INS2_TDTS_NOTAP (0x00 << 2)
// single tap event
#define KX122_INS2_TDTS_SINGLE (0x01 << 2)
// double tap event
#define KX122_INS2_TDTS_DOUBLE (0x02 << 2)
// do not exist
#define KX122_INS2_TDTS_NA (0x03 << 2)
// Status of Wake up. This bit is cleared when the interrupt release register INT_REL is read.
#define KX122_INS2_WUFS (0x01 << 1)
// Tilt Position status. This bit is cleared when the interrupt release register INT_REL is read.
#define KX122_INS2_TPS (0x01 << 0)
// x-
#define KX122_INS3_XNWU (0x01 << 5)
// x+
#define KX122_INS3_XPWU (0x01 << 4)
// y-
#define KX122_INS3_YNWU (0x01 << 3)
// y+
#define KX122_INS3_YPWU (0x01 << 2)
// z-
#define KX122_INS3_ZNWU (0x01 << 1)
// z+
#define KX122_INS3_ZPWU (0x01 << 0)
// INT reports the combined (OR) interrupt information of all features.
#define KX122_STATUS_REG_INT (0x01 << 4)
// controls the operating mode of the KX122.
#define KX122_CNTL1_PC1 (0x01 << 7)
// determines the performance mode of the KX122. The noise varies with ODR, RES and different LP_CNTL settings possibly reducing the effective resolution.
#define KX122_CNTL1_RES (0x01 << 6)
// enables the reporting of the availability of new acceleration data as an interrupt
#define KX122_CNTL1_DRDYE (0x01 << 5)
// 2g range
#define KX122_CNTL1_GSEL_2G (0x00 << 3)
// 4g range
#define KX122_CNTL1_GSEL_4G (0x01 << 3)
// 8g range
#define KX122_CNTL1_GSEL_8G (0x02 << 3)
// not valid settings
#define KX122_CNTL1_GSEL_NA (0x03 << 3)
// enables the Directional Tap function that will detect single and double tap events.
#define KX122_CNTL1_TDTE (0x01 << 2)
// enables the Wake Up (motion detect) function
#define KX122_CNTL1_WUFE (0x01 << 1)
// enables the Tilt Position function that will detect changes in device orientation.
#define KX122_CNTL1_TPE (0x01 << 0)
// initiates software reset, which performs the RAM reboot routine
#define KX122_CNTL2_SRST (0x01 << 7)
// command test control
#define KX122_CNTL2_COTC (0x01 << 6)
// x-
#define KX122_CNTL2_LEM (0x01 << 5)
// x+
#define KX122_CNTL2_RIM (0x01 << 4)
// y-
#define KX122_CNTL2_DOM (0x01 << 3)
// y+
#define KX122_CNTL2_UPM (0x01 << 2)
// z-
#define KX122_CNTL2_FDM (0x01 << 1)
// z+
#define KX122_CNTL2_FUM (0x01 << 0)
// 1.5Hz
#define KX122_CNTL3_OTP_1P563 (0x00 << 6)
// 6.25Hz
#define KX122_CNTL3_OTP_6P25 (0x01 << 6)
// 12.5Hz
#define KX122_CNTL3_OTP_12P5 (0x02 << 6)
// 50Hz
#define KX122_CNTL3_OTP_50 (0x03 << 6)
// 50Hz
#define KX122_CNTL3_OTDT_50 (0x00 << 3)
// 100Hz
#define KX122_CNTL3_OTDT_100 (0x01 << 3)
// 200Hz
#define KX122_CNTL3_OTDT_200 (0x02 << 3)
// 400Hz
#define KX122_CNTL3_OTDT_400 (0x03 << 3)
// 12.5Hz
#define KX122_CNTL3_OTDT_12P5 (0x04 << 3)
// 25Hz
#define KX122_CNTL3_OTDT_25 (0x05 << 3)
// 800Hz
#define KX122_CNTL3_OTDT_800 (0x06 << 3)
// 1600Hz
#define KX122_CNTL3_OTDT_1600 (0x07 << 3)
// 0.78Hz
#define KX122_CNTL3_OWUF_0P781 (0x00 << 0)
// 1.563Hz
#define KX122_CNTL3_OWUF_1P563 (0x01 << 0)
// 3.125Hz
#define KX122_CNTL3_OWUF_3P125 (0x02 << 0)
// 6.25Hz
#define KX122_CNTL3_OWUF_6P25 (0x03 << 0)
// 12.5Hz
#define KX122_CNTL3_OWUF_12P5 (0x04 << 0)
// 25Hz
#define KX122_CNTL3_OWUF_25 (0x05 << 0)
// 50Hz
#define KX122_CNTL3_OWUF_50 (0x06 << 0)
// 100Hz
#define KX122_CNTL3_OWUF_100 (0x07 << 0)
// filtering applied
#define KX122_ODCNTL_IIR_BYPASS_APPLY (0x00 << 7)
// filter bypassed
#define KX122_ODCNTL_IIR_BYPASS_BYPASS (0x01 << 7)
// filter corner frequency set to ODR/9
#define KX122_ODCNTL_LPRO_ODR_9 (0x00 << 6)
// filter corner frequency set to ODR/2
#define KX122_ODCNTL_LPRO_ODR_2 (0x01 << 6)
// 12.5Hz
#define KX122_ODCNTL_OSA_12P5 (0x00 << 0)
// 25Hz
#define KX122_ODCNTL_OSA_25 (0x01 << 0)
// 50Hz
#define KX122_ODCNTL_OSA_50 (0x02 << 0)
// 100Hz
#define KX122_ODCNTL_OSA_100 (0x03 << 0)
// 200Hz
#define KX122_ODCNTL_OSA_200 (0x04 << 0)
// 400Hz
#define KX122_ODCNTL_OSA_400 (0x05 << 0)
// 800Hz
#define KX122_ODCNTL_OSA_800 (0x06 << 0)
// 1600Hz
#define KX122_ODCNTL_OSA_1600 (0x07 << 0)
// 0.78Hz
#define KX122_ODCNTL_OSA_0P781 (0x08 << 0)
// 1.563Hz
#define KX122_ODCNTL_OSA_1P563 (0x09 << 0)
// 3.125Hz
#define KX122_ODCNTL_OSA_3P125 (0x0A << 0)
// 6.25Hz
#define KX122_ODCNTL_OSA_6P25 (0x0B << 0)
// 3200Hz
#define KX122_ODCNTL_OSA_3200 (0x0C << 0)
// 6400Hz
#define KX122_ODCNTL_OSA_6400 (0x0D << 0)
// 12800Hz
#define KX122_ODCNTL_OSA_12800 (0x0E << 0)
// 25600Hz
#define KX122_ODCNTL_OSA_25600 (0x0F << 0)
// pulse 50us, 10us 1600ODR and over
#define KX122_INC1_PWSEL1_50US_10US (0x00 << 6)
// 1*OSA period
#define KX122_INC1_PWSEL1_1XOSA (0x01 << 6)
// 2*OSA period
#define KX122_INC1_PWSEL1_2XOSA (0x02 << 6)
// 4*OSA period
#define KX122_INC1_PWSEL1_4XOSA (0x03 << 6)
// enables/disables the physical interrupt
#define KX122_INC1_IEN1 (0x01 << 5)
// sets the polarity of the physical interrupt pin
#define KX122_INC1_IEA1 (0x01 << 4)
// sets the response of the physical interrupt pin
#define KX122_INC1_IEL1 (0x01 << 3)
// sets the polarity of Self Test
#define KX122_INC1_STPOL (0x01 << 1)
// sets the 3-wire SPI interface
#define KX122_INC1_SPI3E (0x01 << 0)
// OR combination between selected directions
#define KX122_INC2_AOI_OR (0x00 << 6)
// AND combination between selected axes
#define KX122_INC2_AOI_AND (0x01 << 6)
// x negative (x-): 0 = disabled, 1 = enabled
#define KX122_INC2_XNWUE (0x01 << 5)
// x positive (x+): 0 = disabled, 1 = enabled
#define KX122_INC2_XPWUE (0x01 << 4)
// y negative (y-): 0 = disabled, 1 = enabled
#define KX122_INC2_YNWUE (0x01 << 3)
// y positive (y+): 0 = disabled, 1 = enabled
#define KX122_INC2_YPWUE (0x01 << 2)
// z negative (z-): 0 = disabled, 1 = enabled
#define KX122_INC2_ZNWUE (0x01 << 1)
// z positive (z+): 0 = disabled, 1 = enabled
#define KX122_INC2_ZPWUE (0x01 << 0)
// x negative (x-): 0 = disabled, 1 = enabled
#define KX122_INC3_TLEM (0x01 << 5)
// x positive (x+): 0 = disabled, 1 = enabled
#define KX122_INC3_TRIM (0x01 << 4)
// y negative (y-): 0 = disabled, 1 = enabled
#define KX122_INC3_TDOM (0x01 << 3)
// y positive (y+): 0 = disabled, 1 = enabled
#define KX122_INC3_TUPM (0x01 << 2)
// z negative (z-): 0 = disabled, 1 = enabled
#define KX122_INC3_TFDM (0x01 << 1)
// z positive (z+): 0 = disabled, 1 = enabled
#define KX122_INC3_TFUM (0x01 << 0)
// Free fall interrupt reported on physical interrupt INT1
#define KX122_INC4_FFI1 (0x01 << 7)
// Buffer full interrupt reported on physical interrupt pin INT1
#define KX122_INC4_BFI1 (0x01 << 6)
// Watermark interrupt reported on physical interrupt pin INT1
#define KX122_INC4_WMI1 (0x01 << 5)
// Data ready interrupt reported on physical interrupt pin INT1
#define KX122_INC4_DRDYI1 (0x01 << 4)
// Tap/Double Tap interrupt reported on physical interrupt pin INT1
#define KX122_INC4_TDTI1 (0x01 << 2)
// Wake-Up (motion detect) interrupt reported on physical interrupt pin INT1
#define KX122_INC4_WUFI1 (0x01 << 1)
// Tilt position interrupt reported on physical interrupt pin INT1
#define KX122_INC4_TPI1 (0x01 << 0)
// pulse 50us, 10us 1600ODR and over
#define KX122_INC5_PWSEL2_50US_10US (0x00 << 6)
// 1*OSA period
#define KX122_INC5_PWSEL2_1XOSA (0x01 << 6)
// 2*OSA period
#define KX122_INC5_PWSEL2_2XOSA (0x02 << 6)
// 4*OSA period
#define KX122_INC5_PWSEL2_4XOSA (0x03 << 6)
// enables/disables the physical interrupt
#define KX122_INC5_IEN2 (0x01 << 5)
// sets the polarity of the physical interrupt pin
#define KX122_INC5_IEA2 (0x01 << 4)
// sets the response of the physical interrupt pin
#define KX122_INC5_IEL2 (0x01 << 3)
// Interrupt source automatic clear at interup 2 trailing edge
#define KX122_INC5_ACLR2 (0x01 << 1)
// Interrupt source automatic clear at interup 1 trailing edge
#define KX122_INC5_ACLR1 (0x01 << 0)
// FFI2  Free fall interrupt reported on physical interrupt INT2
#define KX122_INC6_FFI2 (0x01 << 7)
// BFI2  Buffer full interrupt reported on physical interrupt pin INT2
#define KX122_INC6_BFI2 (0x01 << 6)
// WMI2 - Watermark interrupt reported on physical interrupt pin INT2
#define KX122_INC6_WMI2 (0x01 << 5)
// DRDYI2  Data ready interrupt reported on physical interrupt pin INT2
#define KX122_INC6_DRDYI2 (0x01 << 4)
// TDTI2 - Tap/Double Tap interrupt reported on physical interrupt pin INT2
#define KX122_INC6_TDTI2 (0x01 << 2)
// WUFI2  Wake-Up (motion detect) interrupt reported on physical interrupt pin INT2
#define KX122_INC6_WUFI2 (0x01 << 1)
// TPI2  Tilt position interrupt reported on physical interrupt pin INT2
#define KX122_INC6_TPI2 (0x01 << 0)
// enables/disables the double tap interrupt
#define KX122_TDTRC_DTRE (0x01 << 1)
// enables/disables single tap interrupt
#define KX122_TDTRC_STRE (0x01 << 0)
// Free fall engine enable
#define KX122_FFCNTL_FFIE (0x01 << 7)
// Free fall interrupt latch/un-latch control
#define KX122_FFCNTL_ULMODE (0x01 << 6)
// Debounce methodology control
#define KX122_FFCNTL_DCRM (0x01 << 3)
// 12.5Hz
#define KX122_FFCNTL_OFFI_12P5 (0x00 << 0)
// 25Hz
#define KX122_FFCNTL_OFFI_25 (0x01 << 0)
// 50Hz
#define KX122_FFCNTL_OFFI_50 (0x02 << 0)
// 100Hz
#define KX122_FFCNTL_OFFI_100 (0x03 << 0)
// 200Hz
#define KX122_FFCNTL_OFFI_200 (0x04 << 0)
// 400Hz
#define KX122_FFCNTL_OFFI_400 (0x05 << 0)
// 800Hz
#define KX122_FFCNTL_OFFI_800 (0x06 << 0)
// 1600Hz
#define KX122_FFCNTL_OFFI_1600 (0x07 << 0)
// No Averaging
#define KX122_LP_CNTL_AVC_NO_AVG (0x00 << 4)
// 2 Samples Averaged
#define KX122_LP_CNTL_AVC_2_SAMPLE_AVG (0x01 << 4)
// 4 Samples Averaged
#define KX122_LP_CNTL_AVC_4_SAMPLE_AVG (0x02 << 4)
// 8 Samples Averaged
#define KX122_LP_CNTL_AVC_8_SAMPLE_AVG (0x03 << 4)
// 16 Samples Averaged (default)
#define KX122_LP_CNTL_AVC_16_SAMPLE_AVG (0x04 << 4)
// 32 Samples Averaged
#define KX122_LP_CNTL_AVC_32_SAMPLE_AVG (0x05 << 4)
// 64 Samples Averaged
#define KX122_LP_CNTL_AVC_64_SAMPLE_AVG (0x06 << 4)
// 128 Samples Averaged
#define KX122_LP_CNTL_AVC_128_SAMPLE_AVG (0x07 << 4)
#define KX122_BUF_CNTL1_SMP_TH0_7 (0xFF << 0)
// controls activation of the sample buffer
#define KX122_BUF_CNTL2_BUFE (0x01 << 7)
// determines the resolution of the acceleration data samples collected by the sample
#define KX122_BUF_CNTL2_BRES (0x01 << 6)
// buffer full interrupt enable bit
#define KX122_BUF_CNTL2_BFIE (0x01 << 5)
// watermark level bits 8 and 9
#define KX122_BUF_CNTL2_SMP_TH8_9 (0x0C << 2)
// The buffer collects 681 sets of 8-bit low resolution values or 339 sets of 16-bit high resolution values and then stops collecting data, collecting new data only when the buffer is not full
#define KX122_BUF_CNTL2_BUF_M_FIFO (0x00 << 0)
// The buffer holds the last 681 sets of 8-bit low resolution values or 339 sets of 16-bit high resolution values. Once the buffer is full, the oldest data is discarded to make room for newer data.
#define KX122_BUF_CNTL2_BUF_M_STREAM (0x01 << 0)
// When a trigger event occurs, the buffer holds the last data set of SMP[9:0] samples before the trigger event and then continues to collect data until full. New data is collected only when the buffer is not full.
#define KX122_BUF_CNTL2_BUF_M_TRIGGER (0x02 << 0)
// The buffer holds the last 681 sets of 8-bit low resolution values or 339 sets of 16-bit high resolution values. Once the buffer is full, the oldest data is discarded to make room for newer data. Reading from the buffer in this mode will return the most recent data first.
#define KX122_BUF_CNTL2_BUF_M_FILO (0x03 << 0)
#define KX122_BUF_STATUS_1_SMP_LEV0_7 (0xFF << 0)
// reports the status of the buffers trigger function if this mode has been selected
#define KX122_BUF_STATUS_2_BUF_TRIG (0x01 << 7)
// level High mask
#define KX122_BUF_STATUS_2_SMP_LEV8_10 (0x07 << 0)
// MEMS Test OFF
#define KX122_SELF_TEST_MEMS_TEST_OFF (0x00 << 0)
// MEMS Test ON
#define KX122_SELF_TEST_MEMS_TEST_ON (0xCA << 0)
// WHO_AM_I -value for KX112
#define KX112_WHO_AM_I_WIA_ID (0x22 << 0)
// WHO_AM_I -value for KX123
#define KX123_WHO_AM_I_WIA_ID (0x20 << 0)
// WHO_AM_I -value for KX124
#define KX124_WHO_AM_I_WIA_ID (0x28 << 0)
 /*registers bit masks */

#define KX122_COTR_DCSTR_MASK 0xFF

#define KX122_WHO_AM_I_WIA_MASK 0xFF
// status of tap/double tap, bit is released when interrupt release register INT_REL is read.
#define KX122_INS2_TDTS_MASK 0x0C
// selects the acceleration range of the accelerometer outputs
#define KX122_CNTL1_GSEL_MASK 0x18
// sets the output data rate for the Tilt Position function
#define KX122_CNTL3_OTP_MASK 0xC0
// sets the output data rate for the Directional TapTM function
#define KX122_CNTL3_OTDT_MASK 0x38
// sets the output data rate for the general motion detection function and the high-pass filtered outputs
#define KX122_CNTL3_OWUF_MASK 0x07
// filter bypass mode
#define KX122_ODCNTL_IIR_BYPASS_MASK 0x80
// low-pass filter roll off control
#define KX122_ODCNTL_LPRO_MASK 0x40
// acceleration output data rate.
#define KX122_ODCNTL_OSA_MASK 0x0F
// Pulse interrupt 1 width configuration
#define KX122_INC1_PWSEL1_MASK 0xC0
// AND OR configuration for motion detection
#define KX122_INC2_AOI_MASK 0x40
#define KX122_INC2_WUE_MASK 0x3F
#define KX122_INC3_TM_MASK 0x3F
// Pulse interrupt 2 width configuration
#define KX122_INC5_PWSEL2_MASK 0xC0
// Output Data Rate at which the Free fall engine performs its function.
#define KX122_FFCNTL_OFFI_MASK 0x07
#define KX122_HYST_SET_HYST_MASK 0x3F
// Averaging Filter Control
#define KX122_LP_CNTL_AVC_MASK 0x70

#define KX122_BUF_CNTL1_SMP_TH0_MASK 0xFF
#define KX122_BUF_CNTL1_SMP_TH0_7_MASK 0xFF

#define KX122_BUF_CNTL2_SMP_TH8_MASK 0x0C
#define KX122_BUF_CNTL2_SMP_TH8_9_MASK 0x0C
// selects the operating mode of the sample buffer
#define KX122_BUF_CNTL2_BUF_M_MASK 0x03

#define KX122_BUF_STATUS_1_SMP_LEV0_MASK 0xFF
#define KX122_BUF_STATUS_1_SMP_LEV0_7_MASK 0xFF

#define KX122_BUF_STATUS_2_SMP_LEV8_MASK 0x07
#define KX122_BUF_STATUS_2_SMP_LEV8_10_MASK 0x07

#define KX122_SELF_TEST_MEMS_TEST_MASK 0xFF

#define KX112_WHO_AM_I_WIA_MASK 0xFF

#define KX123_WHO_AM_I_WIA_MASK 0xFF

#define KX124_WHO_AM_I_WIA_MASK 0xFF
#endif

