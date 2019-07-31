/*
 * Author: Alex Tereschenko <alext.mkrs@gmail.com>
 * Copyright (c) 2018 Alex Tereschenko.
 *
 * Based on LIS2DS12 module by
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * The MIT License
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define LIS3DH_DEFAULT_I2C_BUS 0
#define LIS3DH_DEFAULT_SPI_BUS 0
#define LIS3DH_DEFAULT_I2C_ADDR 0x18

#define LIS3DH_CHIPID 0x33

// NOTE: Reserved registers must not be written into or permanent
// damage can result. Reading from them may return indeterminate
// values. Registers containing reserved bitfields must be
// written as 0, except for CTRL_REG0 - see datasheet for that one.

/**
 * LIS3DH registers
 */
typedef enum {
    // 0x00-0x06 reserved

    LIS3DH_REG_STATUS_REG_AUX = 0x07,

    LIS3DH_REG_OUT_ADC1_L = 0x08,
    LIS3DH_REG_OUT_ADC1_H = 0x09,
    LIS3DH_REG_OUT_ADC2_L = 0x0A,
    LIS3DH_REG_OUT_ADC2_H = 0x0B,
    LIS3DH_REG_OUT_ADC3_L = 0x0C,
    LIS3DH_REG_OUT_ADC3_H = 0x0D,

    // 0x0E reserved

    LIS3DH_REG_WHO_AM_I = 0x0F,

    // 0x10-0x1D reserved

    LIS3DH_REG_CTRL_REG0 = 0x1E,
    LIS3DH_REG_TEMP_CFG_REG = 0x1F,
    LIS3DH_REG_CTRL_REG1 = 0x20,
    LIS3DH_REG_CTRL_REG2 = 0x21,
    LIS3DH_REG_CTRL_REG3 = 0x22,
    LIS3DH_REG_CTRL_REG4 = 0x23,
    LIS3DH_REG_CTRL_REG5 = 0x24,
    LIS3DH_REG_CTRL_REG6 = 0x25,

    LIS3DH_REG_REFERENCE = 0x26,

    LIS3DH_REG_STATUS_REG = 0x27,

    LIS3DH_REG_OUT_X_L = 0x28,
    LIS3DH_REG_OUT_X_H = 0x29,
    LIS3DH_REG_OUT_Y_L = 0x2A,
    LIS3DH_REG_OUT_Y_H = 0x2B,
    LIS3DH_REG_OUT_Z_L = 0x2C,
    LIS3DH_REG_OUT_Z_H = 0x2D,

    LIS3DH_REG_FIFO_CTRL_REG = 0x2E,
    LIS3DH_REG_FIFO_SRC_REG = 0x2F,

    LIS3DH_REG_INT1_CFG = 0x30,
    LIS3DH_REG_INT1_SRC = 0x31,
    LIS3DH_REG_INT1_THS = 0x32,
    LIS3DH_REG_INT1_DURATION = 0x33,

    LIS3DH_REG_INT2_CFG = 0x34,
    LIS3DH_REG_INT2_SRC = 0x35,
    LIS3DH_REG_INT2_THS = 0x36,
    LIS3DH_REG_INT2_DURATION = 0x37,

    LIS3DH_REG_CLICK_CFG = 0x38,
    LIS3DH_REG_CLICK_SRC = 0x39,
    LIS3DH_REG_CLICK_THS = 0x3A,

    LIS3DH_REG_TIME_LIMIT = 0x3B,
    LIS3DH_REG_TIME_LATENCY = 0x3C,
    LIS3DH_REG_TIME_WINDOW = 0x3D,

    LIS3DH_REG_ACT_THS = 0x3E,
    LIS3DH_REG_ACT_DUR = 0x3F,
} LIS3DH_REGS_T;

/**
 * REG_STATUS_REG_AUX bits
 */
typedef enum {
    LIS3DH_STATUS_REG_AUX_1DA = 0x01,
    LIS3DH_STATUS_REG_AUX_2DA = 0x02,
    LIS3DH_STATUS_REG_AUX_3DA = 0x04,
    LIS3DH_STATUS_REG_AUX_321DA = 0x08,

    LIS3DH_STATUS_REG_AUX_1OR = 0x10,
    LIS3DH_STATUS_REG_AUX_2OR = 0x20,
    LIS3DH_STATUS_REG_AUX_3OR = 0x40,
    LIS3DH_STATUS_REG_AUX_321OR = 0x80,
} LIS3DH_STATUS_REG_AUX_BITS_T;

/**
 * REG_CTRL_REG0 bits
 */
typedef enum {
    // 0x01-0x40 reserved

    LIS3DH_CTRL_REG0_SDO_PU_DISC = 0x80,
} LIS3DH_CTRL_REG0_BITS_T;

/**
 * REG_TEMP_CFG_REG bits
 */
typedef enum {
    // 0x01-0x20 reserved

    LIS3DH_TEMP_CFG_REG_TEMP_EN = 0x40,
    LIS3DH_TEMP_CFG_REG_ADC_EN = 0x80,
} LIS3DH_TEMP_CFG_REG_BITS_T;

/**
 * REG_CTRL_REG1 bits
 */
typedef enum {
    LIS3DH_CTRL_REG1_XEN = 0x01,
    LIS3DH_CTRL_REG1_YEN = 0x02,
    LIS3DH_CTRL_REG1_ZEN = 0x04,
    LIS3DH_CTRL_REG1_LPEN = 0x08,

    LIS3DH_CTRL_REG1_ODR0 = 0x10,
    LIS3DH_CTRL_REG1_ODR1 = 0x20,
    LIS3DH_CTRL_REG1_ODR2 = 0x40,
    LIS3DH_CTRL_REG1_ODR3 = 0x80,
    _LIS3DH_CTRL_REG1_ODR_MASK = 0xF,
    _LIS3DH_CTRL_REG1_ODR_SHIFT = 4,
} LIS3DH_CTRL_REG1_BITS_T;

/**
 * CTRL_REG1_ODR (Output Data Rate) values
 */
typedef enum {
    LIS3DH_ODR_POWER_DOWN = 0x0, // 0b0000

    // These are allowed in all modes (high resolution/normal/low power)
    LIS3DH_ODR_1HZ = 0x1,   // 0b0001
    LIS3DH_ODR_10HZ = 0x2,  // 0b0010
    LIS3DH_ODR_25HZ = 0x3,  // 0b0011
    LIS3DH_ODR_50HZ = 0x4,  // 0b0100
    LIS3DH_ODR_100HZ = 0x5, // 0b0101
    LIS3DH_ODR_200HZ = 0x6, // 0b0110
    LIS3DH_ODR_400HZ = 0x7, // 0b0111

    // The following two items have a dual meaning depending
    // on whether the LPEN bit is set, but they use the same
    // overlapping ODR values for the ODR bitfield. Since the
    // bitfield is only 4 bits wide, we add a "virtual" 5th bit to
    // indicate the LP versions. This is then screened out in the
    // code and will set the LPEN bit according to what is selected
    // here.

    // CTRL_REG1_LPEN == 0 (high resolution/normal mode)
    LIS3DH_ODR_1344HZ = 0x9, // 0b1001

    // CTRL_REG1_LPEN == 1 (low power mode). Add 'virtual' bit 5
    // value (0x10) for these LP modes, which we will detect and
    // screen out in the driver. This simplifies the ODR API.
    LIS3DH_ODR_5376HZ = (0x10 + 0x9), // 0b1001

    // Low power-only mode, requires LPEN == 1, so add the virtual bit
    LIS3DH_ODR_LP_1600HZ = (0x10 + 0x8), // 0b1000
} LIS3DH_ODR_T;

/**
 * REG_CTRL_REG2 bits
 */
typedef enum {
    LIS3DH_CTRL_REG2_HP_IA1 = 0x01,
    LIS3DH_CTRL_REG2_HP_IA2 = 0x02,
    LIS3DH_CTRL_REG2_HPCLICK = 0x04,
    LIS3DH_CTRL_REG2_FDS = 0x08,

    LIS3DH_CTRL_REG2_HPCF0 = 0x10,
    LIS3DH_CTRL_REG2_HPCF1 = 0x20,
    _LIS3DH_CTRL_REG2_HPCF_MASK = 0x3, // 0b11
    _LIS3DH_CTRL_REG2_HPCF_SHIFT = 4,

    LIS3DH_CTRL_REG2_HPM0 = 0x40,
    LIS3DH_CTRL_REG2_HPM1 = 0x80,
    _LIS3DH_CTRL_REG2_HPM_MASK = 0x3, // 0b11
    _LIS3DH_CTRL_REG2_HPM_SHIFT = 6,
} LIS3DH_CTRL_REG2_BITS_T;

/**
 * CTRL_REG2_HPM (High-pass filter mode) values
 */
typedef enum {
    LIS3DH_HPM_NORMAL_RST_REF_READ = 0x0, // 0b00, Normal mode (reset by reading REFERENCE)
    LIS3DH_HPM_REF_SIGNAL = 0x1,          // 0b01, Reference signal for filtering
    LIS3DH_HPM_NORMAL = 0x2,              // 0b10, Normal mode
    LIS3DH_HPM_AUTORST = 0x3,             // 0b11, Autoreset on interrupt event
} LIS3DH_HPM_T;

/**
 * REG_CTRL_REG3 bits
 */
typedef enum {
    // 0x01 reserved

    LIS3DH_CTRL_REG3_I1_OVERRUN = 0x02,
    LIS3DH_CTRL_REG3_I1_WTM = 0x04,
    LIS3DH_CTRL_REG3_I1_321DA = 0x08,
    LIS3DH_CTRL_REG3_I1_ZYXDA = 0x10,
    LIS3DH_CTRL_REG3_I1_IA2 = 0x20,
    LIS3DH_CTRL_REG3_I1_IA1 = 0x40,
    LIS3DH_CTRL_REG3_I1_CLICK = 0x80,
} LIS3DH_CTRL_REG3_BITS_T;

/**
 * REG_CTRL_REG4 bits
 */
typedef enum {
    LIS3DH_CTRL_REG4_SIM = 0x01,

    LIS3DH_CTRL_REG4_ST0 = 0x02,
    LIS3DH_CTRL_REG4_ST1 = 0x04,
    _LIS3DH_CTRL_REG4_ST_MASK = 0x3, // 0b11
    _LIS3DH_CTRL_REG4_ST_SHIFT = 1,

    LIS3DH_CTRL_REG4_HR = 0x08,

    LIS3DH_CTRL_REG4_FS0 = 0x10,
    LIS3DH_CTRL_REG4_FS1 = 0x20,
    _LIS3DH_CTRL_REG4_FS_MASK = 0x3, // 0b11
    _LIS3DH_CTRL_REG4_FS_SHIFT = 4,

    LIS3DH_CTRL_REG4_BLE = 0x40,
    LIS3DH_CTRL_REG4_BDU = 0x80,
} LIS3DH_CTRL_REG4_BITS_T;

/**
 * CTRL_REG4_ST (Self Test) values
 */
typedef enum {
    LIS3DH_ST_NORMAL = 0x0, // 0b00
    LIS3DH_ST_ST0 = 0x1,    // 0b01
    LIS3DH_ST_ST1 = 0x2,    // 0b10
} LIS3DH_ST_T;

/**
 * CTRL_REG4_FS (Full Scale) values
 */
typedef enum {
    LIS3DH_FS_2G = 0x0,  // 0b00
    LIS3DH_FS_4G = 0x1,  // 0b01
    LIS3DH_FS_8G = 0x2,  // 0b10
    LIS3DH_FS_16G = 0x3, // 0b11
} LIS3DH_FS_T;

/**
 * REG_CTRL_REG5 bits
 */
typedef enum {
    LIS3DH_CTRL_REG5_D4D_INT2 = 0x01,
    LIS3DH_CTRL_REG5_LIR_INT2 = 0x02,
    LIS3DH_CTRL_REG5_D4D_INT1 = 0x04,
    LIS3DH_CTRL_REG5_LIR_INT1 = 0x08,

    // 0x10-0x20 reserved

    LIS3DH_CTRL_REG5_FIFO_EN = 0x40,
    LIS3DH_CTRL_REG5_BOOT = 0x80,
} LIS3DH_CTRL_REG5_BITS_T;

/**
 * REG_CTRL_REG6 bits
 */
typedef enum {
    // 0x01 reserved

    LIS3DH_CTRL_REG6_INT_POLARITY = 0x02,

    // 0x04 reserved

    LIS3DH_CTRL_REG6_I2_ACT = 0x08,
    LIS3DH_CTRL_REG6_I2_BOOT = 0x10,
    LIS3DH_CTRL_REG6_I2_IA2 = 0x20,
    LIS3DH_CTRL_REG6_I2_IA1 = 0x40,
    LIS3DH_CTRL_REG6_I2_CLICK = 0x80,
} LIS3DH_CTRL_REG6_BITS_T;

/**
 * REG_STATUS_REG bits
 */
typedef enum {
    LIS3DH_STATUS_REG_XDA = 0x01,
    LIS3DH_STATUS_REG_YDA = 0x02,
    LIS3DH_STATUS_REG_ZDA = 0x04,
    LIS3DH_STATUS_REG_ZYXDA = 0x08,

    LIS3DH_STATUS_REG_XOR = 0x10,
    LIS3DH_STATUS_REG_YOR = 0x20,
    LIS3DH_STATUS_REG_ZOR = 0x40,
    LIS3DH_STATUS_REG_ZYXOR = 0x80,
} LIS3DH_STATUS_REG_BITS_T;

/**
 * REG_FIFO_CTRL_REG bits
 */
typedef enum {
    LIS3DH_FIFO_CTRL_REG_FTH0 = 0x01,
    LIS3DH_FIFO_CTRL_REG_FTH1 = 0x02,
    LIS3DH_FIFO_CTRL_REG_FTH2 = 0x04,
    LIS3DH_FIFO_CTRL_REG_FTH3 = 0x08,
    LIS3DH_FIFO_CTRL_REG_FTH4 = 0x10,
    _LIS3DH_FIFO_CTRL_REG_FTH_MASK = 0x1F, // 0b11111
    _LIS3DH_FIFO_CTRL_REG_FTH_SHIFT = 0,

    LIS3DH_FIFO_CTRL_REG_TR = 0x20,

    LIS3DH_FIFO_CTRL_REG_FM0 = 0x40,
    LIS3DH_FIFO_CTRL_REG_FM1 = 0x80,
    _LIS3DH_FIFO_CTRL_REG_FM_MASK = 0x3, // 0b11
    _LIS3DH_FIFO_CTRL_REG_FM_SHIFT = 6,
} LIS3DH_FIFO_CTRL_REG_BITS_T;

/**
 * FIFO_CTRL_REG_FM (FIFO Mode) values
 */
typedef enum {
    LIS3DH_FM_BYPASS = 0x0,         // 0b00
    LIS3DH_FM_FIFO = 0x1,           // 0b01
    LIS3DH_FM_STREAM = 0x2,         // 0b10
    LIS3DH_FM_STREAM_TO_FIFO = 0x3, // 0b11
} LIS3DH_FM_T;

/**
 * REG_FIFO_SRC_REG bits
 */
typedef enum {
    LIS3DH_FIFO_SRC_REG_FSS0 = 0x01,
    LIS3DH_FIFO_SRC_REG_FSS1 = 0x02,
    LIS3DH_FIFO_SRC_REG_FSS2 = 0x04,
    LIS3DH_FIFO_SRC_REG_FSS3 = 0x08,
    LIS3DH_FIFO_SRC_REG_FSS4 = 0x10,
    _LIS3DH_FIFO_SRC_REG_FSS_MASK = 0x1F, // 0b11111
    _LIS3DH_FIFO_SRC_REG_FSS_SHIFT = 0,

    LIS3DH_FIFO_SRC_REG_EMPTY = 0x20,
    LIS3DH_FIFO_SRC_REG_OVRN_FIFO = 0x40,
    LIS3DH_FIFO_SRC_REG_WTM = 0x80,
} LIS3DH_FIFO_SRC_REG_BITS_T;

/**
 * REG_INT1_CFG bits
 */
typedef enum {
    LIS3DH_INT1_CFG_XLIE = 0x01,
    LIS3DH_INT1_CFG_XHIE = 0x02,

    LIS3DH_INT1_CFG_YLIE = 0x04,
    LIS3DH_INT1_CFG_YHIE = 0x08,

    LIS3DH_INT1_CFG_ZLIE = 0x10,
    LIS3DH_INT1_CFG_ZHIE = 0x20,

    LIS3DH_INT1_CFG_6D = 0x40,
    LIS3DH_INT1_CFG_AOI = 0x80,
} LIS3DH_INT1_CFG_BITS_T;

/**
 * REG_INT1_SRC bits
 */
typedef enum {
    LIS3DH_INT1_SRC_XL = 0x01,
    LIS3DH_INT1_SRC_XH = 0x02,

    LIS3DH_INT1_SRC_YL = 0x04,
    LIS3DH_INT1_SRC_YH = 0x08,

    LIS3DH_INT1_SRC_ZL = 0x10,
    LIS3DH_INT1_SRC_ZH = 0x20,

    LIS3DH_INT1_SRC_IA = 0x40,

    // 0x80 reserved
} LIS3DH_INT1_SRC_BITS_T;

/**
 * REG_INT2_CFG bits
 */
typedef enum {
    LIS3DH_INT2_CFG_XLIE = 0x01,
    LIS3DH_INT2_CFG_XHIE = 0x02,

    LIS3DH_INT2_CFG_YLIE = 0x04,
    LIS3DH_INT2_CFG_YHIE = 0x08,

    LIS3DH_INT2_CFG_ZLIE = 0x10,
    LIS3DH_INT2_CFG_ZHIE = 0x20,

    LIS3DH_INT2_CFG_6D = 0x40,
    LIS3DH_INT2_CFG_AOI = 0x80,
} LIS3DH_INT2_CFG_BITS_T;

/**
 * REG_INT2_SRC bits
 */
typedef enum {
    LIS3DH_INT2_SRC_XL = 0x01,
    LIS3DH_INT2_SRC_XH = 0x02,

    LIS3DH_INT2_SRC_YL = 0x04,
    LIS3DH_INT2_SRC_YH = 0x08,

    LIS3DH_INT2_SRC_ZL = 0x10,
    LIS3DH_INT2_SRC_ZH = 0x20,

    LIS3DH_INT2_SRC_IA = 0x40,

    // 0x80 reserved
} LIS3DH_INT2_SRC_BITS_T;

/**
 * REG_CLICK_CFG bits
 */
typedef enum {
    LIS3DH_CLICK_CFG_XS = 0x01,
    LIS3DH_CLICK_CFG_XD = 0x02,

    LIS3DH_CLICK_CFG_YS = 0x04,
    LIS3DH_CLICK_CFG_YD = 0x08,

    LIS3DH_CLICK_CFG_ZS = 0x10,
    LIS3DH_CLICK_CFG_ZD = 0x20,

    // 0x40-0x80 reserved
} LIS3DH_CLICK_CFG_BITS_T;

/**
 * REG_CLICK_SRC bits
 */
typedef enum {
    LIS3DH_CLICK_SRC_X = 0x01,
    LIS3DH_CLICK_SRC_Y = 0x02,
    LIS3DH_CLICK_SRC_Z = 0x04,

    LIS3DH_CLICK_SRC_SIGN = 0x08,

    LIS3DH_CLICK_SRC_SCLICK = 0x10,
    LIS3DH_CLICK_SRC_DCLICK = 0x20,

    LIS3DH_CLICK_SRC_IA = 0x40,

    // 0x80 reserved
} LIS3DH_CLICK_SRC_BITS_T;

/**
 * REG_CLICK_THS bits
 */
typedef enum {
    LIS3DH_CLICK_THS_THS0 = 0x01,
    LIS3DH_CLICK_THS_THS1 = 0x02,
    LIS3DH_CLICK_THS_THS2 = 0x04,
    LIS3DH_CLICK_THS_THS3 = 0x08,
    LIS3DH_CLICK_THS_THS4 = 0x10,
    LIS3DH_CLICK_THS_THS5 = 0x20,
    LIS3DH_CLICK_THS_THS6 = 0x40,
    _LIS3DH_CLICK_THS_THS_MASK = 0x7F, // 0b1111111
    _LIS3DH_CLICK_THS_THS_SHIFT = 0,

    LIS3DH_CLICK_THS_LIR_CLICK = 0x80,
} LIS3DH_CLICK_THS_BITS_T;

// Interrupt selection for installISR() and uninstallISR()
typedef enum { LIS3DH_INTERRUPT_INT1, LIS3DH_INTERRUPT_INT2 } LIS3DH_INTERRUPT_PINS_T;

#ifdef __cplusplus
}
#endif
