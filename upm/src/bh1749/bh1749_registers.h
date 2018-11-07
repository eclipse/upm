/*
The MIT License (MIT)
Copyright (c) 2017 Rohm Semiconductor

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

#ifndef __BH1749_REGISTERS_H__
#define __BH1749_REGISTERS_H__
/* registers */
#define BH1749_REGISTER_DUMP_START 0x40
#define BH1749_SYSTEM_CONTROL 0x40
#define BH1749_MODE_CONTROL1 0x41
#define BH1749_MODE_CONTROL2 0x42
// Least significant byte of uint16 RED measurement value
#define BH1749_RED_DATA_LSBS 0x50
// Most significant byte of uint16 RED measurement value
#define BH1749_RED_DATA_MSBS 0x51
// Least significant byte of uint16 GREEN measurement value
#define BH1749_GREEN_DATA_LSBS 0x52
// Most significant byte of uint16 GREEN measurement value
#define BH1749_GREEN_DATA_MSBS 0x53
// Least significant byte of uint16 BLUE measurement value
#define BH1749_BLUE_DATA_LSBS 0x54
// Most significant byte of uint16 BLUE measurement value
#define BH1749_BLUE_DATA_MSBS 0x55
// Least significant byte of uint16 IR measurement value
#define BH1749_IR_DATA_LSBS 0x58
// Most significant byte of uint16 IR measurement value
#define BH1749_IR_DATA_MSBS 0x59
// Least significant byte of uint16 GREEN2 measurement value
#define BH1749_GREEN2_DATA_LSBS 0x5A
// Most significant byte of uint16 GREEN2 measurement value
#define BH1749_GREEN2_DATA_MSBS 0x5B
// Interrupt control register
#define BH1749_INTERRUPT 0x60
// Interrupt status update control register
#define BH1749_PERSISTENCE 0x61
// Least significant byte of interrupt threshold high level
#define BH1749_TH_LSBS 0x62
// Most significant byte of interrupt threshold high level
#define BH1749_TH_MSBS 0x63
// Least significant byte of interrupt threshold low level
#define BH1749_TL_LSBS 0x64
// Most significant byte of interrupt threshold low level
#define BH1749_TL_MSBS 0x65
#define BH1749_ID_REG 0x92
#define BH1749_REGISTER_DUMP_END 0x92
/* registers bits */
// Software reset is not done
#define BH1749_SYSTEM_CONTROL_SW_RESET_NOT_DONE (0x00 << 7)
// Software reset is done
#define BH1749_SYSTEM_CONTROL_SW_RESET_DONE (0x01 << 7)
// INT pin status is not changed
#define BH1749_SYSTEM_CONTROL_INT_NO_ACTION (0x00 << 6)
// INT pin becomes inactive (high impedance)
#define BH1749_SYSTEM_CONTROL_INT_RESET (0x01 << 6)
#define BH1749_SYSTEM_CONTROL_PART_ID (0x0D << 0)
#define BH1749_MODE_CONTROL1_RESERVED7_WRITE0 (0x00 << 7)
#define BH1749_MODE_CONTROL1_IR_GAIN_RESERVED0 (0x00 << 5)
#define BH1749_MODE_CONTROL1_IR_GAIN_1X (0x01 << 5)
#define BH1749_MODE_CONTROL1_IR_GAIN_RESERVED1 (0x02 << 5)
#define BH1749_MODE_CONTROL1_IR_GAIN_32X (0x03 << 5)
#define BH1749_MODE_CONTROL1_RGB_GAIN_RESERVED0 (0x00 << 3)
#define BH1749_MODE_CONTROL1_RGB_GAIN_1X (0x01 << 3)
#define BH1749_MODE_CONTROL1_RGB_GAIN_RESERVED1 (0x02 << 3)
#define BH1749_MODE_CONTROL1_RGB_GAIN_32X (0x03 << 3)
// Reserved value
#define BH1749_MODE_CONTROL1_ODR_RESERVED0 (0x00 << 0)
// Reserved value
#define BH1749_MODE_CONTROL1_ODR_RESERVED1 (0x01 << 0)
// 120ms measurement time
#define BH1749_MODE_CONTROL1_ODR_8P333 (0x02 << 0)
// 240ms measurement time
#define BH1749_MODE_CONTROL1_ODR_4P167 (0x03 << 0)
// Reserved value
#define BH1749_MODE_CONTROL1_ODR_RESERVED2 (0x04 << 0)
// 35ms measurement time
#define BH1749_MODE_CONTROL1_ODR_28P6 (0x05 << 0)
// Reserved value
#define BH1749_MODE_CONTROL1_ODR_RESERVED3 (0x06 << 0)
// Reserved value
#define BH1749_MODE_CONTROL1_ODR_RESERVED4 (0x07 << 0)
#define BH1749_MODE_CONTROL2_VALID_NO (0x00 << 7)
#define BH1749_MODE_CONTROL2_VALID_YES (0x01 << 7)
#define BH1749_MODE_CONTROL2_RESERVED65_WRITE00 (0x00 << 5)
#define BH1749_MODE_CONTROL2_RGB_MEASUREMENT_INACTIVE (0x00 << 4)
#define BH1749_MODE_CONTROL2_RGB_MEASUREMENT_ACTIVE (0x01 << 4)
#define BH1749_MODE_CONTROL2_RESERVED30_WRITE0000 (0x00 << 0)
#define BH1749_INTERRUPT_STATUS_INACTIVE (0x00 << 7)
#define BH1749_INTERRUPT_STATUS_ACTIVE (0x01 << 7)
#define BH1749_INTERRUPT_RESERVED64_WRITE000 (0x00 << 4)
// red channel
#define BH1749_INTERRUPT_SOURCE_SELECT_RED (0x00 << 2)
// green channel
#define BH1749_INTERRUPT_SOURCE_SELECT_GREEN (0x01 << 2)
// blue channel
#define BH1749_INTERRUPT_SOURCE_SELECT_BLUE (0x02 << 2)
// Reserved value
#define BH1749_INTERRUPT_SOURCE_RESERVED0 (0x03 << 2)
#define BH1749_INTERRUPT_RESERVED1_WRITE0 (0x00 << 1)
#define BH1749_INTERRUPT_EN_DISABLE (0x00 << 0)
#define BH1749_INTERRUPT_EN_ENABLE (0x01 << 0)
#define BH1749_PERSISTENCE_RESERVED72_WRITE000000 (0x00 << 2)
// Interrupt status becomes active at each measurement end.
#define BH1749_PERSISTENCE_MODE_STATUS_ACTIVE_AFTER_MEASUREMENT (0x00 << 0)
// Interrupt status is updated at each measurement end.
#define BH1749_PERSISTENCE_MODE_STATUS_UPDATE_AFTER_MEASUREMENT (0x01 << 0)
// Interrupt status is updated if 4 consecutive threshold judgements are the same
#define BH1749_PERSISTENCE_MODE_STATUS_UPDATE_AFTER_4_SAME (0x02 << 0)
// Interrupt status is updated if 8 consecutive threshold judgements are the same
#define BH1749_PERSISTENCE_MODE_STATUS_UPDATE_AFTER_8_SAME (0x03 << 0)
// Manufacturer ID
#define BH1749_ID_REG_MANUFACTURER_ID (0xE0 << 0)
 /*registers bit masks */
#define BH1749_SYSTEM_CONTROL_SW_RESET_MASK 0x80

#define BH1749_SYSTEM_CONTROL_INT_MASK 0x40

#define BH1749_SYSTEM_CONTROL_PART_MASK 0x3F
#define BH1749_MODE_CONTROL1_RESERVED7_MASK 0x80
#define BH1749_MODE_CONTROL1_IR_GAIN_MASK 0x60
#define BH1749_MODE_CONTROL1_RGB_GAIN_MASK 0x18
#define BH1749_MODE_CONTROL1_ODR_MASK 0x07
// Measurement data update flag. Sets to 0 if MODE_CONTROL1/2 reg,
// MODE_CONTROL2, INTERRUPT, T(H/L)_(LSB/MSB) is written or
// MODE_CONTROL2 read. In specification named as VALID.
#define BH1749_MODE_CONTROL2_VALID_MASK 0x80
#define BH1749_MODE_CONTROL2_RESERVED65_MASK 0x60
#define BH1749_MODE_CONTROL2_RGB_MEASUREMENT_MASK 0x10
#define BH1749_MODE_CONTROL2_RESERVED30_MASK 0x0F
#define BH1749_INTERRUPT_STATUS_MASK 0x80
#define BH1749_INTERRUPT_RESERVED64_MASK 0x70
#define BH1749_INTERRUPT_SOURCE_MASK 0x0C
// Write 0
#define BH1749_INTERRUPT_RESERVED1_MASK 0x02
#define BH1749_INTERRUPT_EN_MASK 0x01
#define BH1749_PERSISTENCE_RESERVED72_MASK 0xFC
#define BH1749_PERSISTENCE_MODE_MASK 0x03

#define BH1749_ID_REG_MANUFACTURER_MASK 0xFF
#endif

