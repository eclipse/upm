/*
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


#define LSM303AGR_DEFAULT_I2C_BUS 0
#define LSM303AGR_DEFAULT_ACC_ADDR 0x19
#define LSM303AGR_DEFAULT_MAG_ADDR 0x1e

// from the WHO_AM_I_* registers
#define LSM303AGR_CHIPID_ACC 0x33
#define LSM303AGR_CHIPID_MAG 0x40

// This device has 2 I2C addresses - one for the accelerometer (ACC)
// and one for the magnetometer (MAG). But, it uses a single register
// map.  The MAG registers start at 0x40, while the ACC registers
// start at 0 and go to 0x3f.  We define the cutoff point here so the
// register access functions can "automatically" choose the correct
// i2c context (MAG or ACC) to use depending on what register is
// being accessed.

#define LSM303AGR_MAX_ACC_ADDR 0x3f


    // NOTE: Reserved registers must not be written into or permanent
    // damage to the device can result.  Reading from them may return
    // indeterminate values.  Registers containing reserved bitfields
    // must be written as 0.

    /**
     * LSM303AGR registers
     */
    typedef enum {

        // accelerometer registers

        // 0x00-0x06 reserved

        LSM303AGR_REG_STATUS_REG_AUX_A             = 0x07,

        // 0x08-0x0b reserved

        LSM303AGR_REG_OUT_TEMP_L_A                 = 0x0c,
        LSM303AGR_REG_OUT_TEMP_H_A                 = 0x0d,

        LSM303AGR_REG_INT_COUNTER_REG_A            = 0x0e,

        LSM303AGR_REG_WHO_AM_I_A                   = 0x0f,

        // 0x10-0x1e reserved

        LSM303AGR_REG_TEMP_CFG_REG_A               = 0x1f,

        LSM303AGR_REG_CTRL_REG1_A                  = 0x20,
        LSM303AGR_REG_CTRL_REG2_A                  = 0x21,
        LSM303AGR_REG_CTRL_REG3_A                  = 0x22,
        LSM303AGR_REG_CTRL_REG4_A                  = 0x23,
        LSM303AGR_REG_CTRL_REG5_A                  = 0x24,
        LSM303AGR_REG_CTRL_REG6_A                  = 0x25,

        LSM303AGR_REG_REFERENCE                    = 0x26,

        LSM303AGR_REG_STATUS_REG_A                 = 0x27,

        LSM303AGR_REG_OUT_X_L_A                    = 0x28,
        LSM303AGR_REG_OUT_X_H_A                    = 0x29,
        LSM303AGR_REG_OUT_Y_L_A                    = 0x2a,
        LSM303AGR_REG_OUT_Y_H_A                    = 0x2b,
        LSM303AGR_REG_OUT_Z_L_A                    = 0x2c,
        LSM303AGR_REG_OUT_Z_H_A                    = 0x2d,

        LSM303AGR_REG_FIFO_CTRL_REG_A              = 0x2e,
        LSM303AGR_REG_FIFO_SRC_REG_A               = 0x2f,

        LSM303AGR_REG_INT1_CFG_A                   = 0x30,
        LSM303AGR_REG_INT1_SRC_A                   = 0x31,
        LSM303AGR_REG_INT1_THS_A                   = 0x32,
        LSM303AGR_REG_INT1_DUR_A                   = 0x33,

        LSM303AGR_REG_INT2_CFG_A                   = 0x34,
        LSM303AGR_REG_INT2_SRC_A                   = 0x35,
        LSM303AGR_REG_INT2_THS_A                   = 0x36,
        LSM303AGR_REG_INT2_DUR_A                   = 0x37,

        LSM303AGR_REG_CLICK_CFG_A                  = 0x38,
        LSM303AGR_REG_CLICK_SRC_A                  = 0x39,
        LSM303AGR_REG_CLICK_THS_A                  = 0x3a,

        LSM303AGR_REG_TIME_LIMIT                   = 0x3b,
        LSM303AGR_REG_TIME_LATENCY_A               = 0x3c,
        LSM303AGR_REG_TIME_WINDOW_A                = 0x3d,

        LSM303AGR_REG_ACT_THS_A                    = 0x3e,
        LSM303AGR_REG_ACT_DUR_A                    = 0x3f,

        // magnetometer registers

        // 0x40-0x44 reserved

        LSM303AGR_REG_OFFSET_X_REG_L_M             = 0x45,
        LSM303AGR_REG_OFFSET_X_REG_H_M             = 0x46,
        LSM303AGR_REG_OFFSET_Y_REG_L_M             = 0x47,
        LSM303AGR_REG_OFFSET_Y_REG_H_M             = 0x48,
        LSM303AGR_REG_OFFSET_Z_REG_L_M             = 0x49,
        LSM303AGR_REG_OFFSET_Z_REG_H_M             = 0x4a,

        // 0x4b-0x4d reserved

        LSM303AGR_REG_WHO_AM_I_M                   = 0x4f,

        // 0x50-0x5f reserved

        LSM303AGR_REG_CFG_REG_A_M                  = 0x60,
        LSM303AGR_REG_CFG_REG_B_M                  = 0x61,
        LSM303AGR_REG_CFG_REG_C_M                  = 0x62,

        LSM303AGR_REG_INT_CTRL_REG_M               = 0x63,
        LSM303AGR_REG_INT_SRC_REG_M                = 0x64,
        LSM303AGR_REG_INT_THS_L_REG_M              = 0x65,
        LSM303AGR_REG_INT_THS_H_REG_M              = 0x66,

        LSM303AGR_REG_STATUS_REG_M                 = 0x67,

        LSM303AGR_REG_OUTX_L_REG_M                 = 0x68,
        LSM303AGR_REG_OUTX_H_REG_M                 = 0x69,
        LSM303AGR_REG_OUTY_L_REG_M                 = 0x6a,
        LSM303AGR_REG_OUTY_H_REG_M                 = 0x6b,
        LSM303AGR_REG_OUTZ_L_REG_M                 = 0x6c,
        LSM303AGR_REG_OUTZ_H_REG_M                 = 0x6d

        // 0x6e-0x6f reserved
    } LSM303AGR_REGS_T;

    // Accelerometer registers

    /**
     * STATUS_REG_AUX_A bits
     */
    typedef enum {
        // 0x01-0x02 reserved

        LSM303AGR_STATUS_REG_AUX_A_TDA             = 0x04, // temp data avail

        // 0x08-0x20 reserved

        LSM303AGR_STATUS_REG_AUX_A_TOR             = 0x40, // temp data overrun

        // 0x80 reserved
    } LSM303AGR_STATUS_REG_AUX_A_BITS_T;

    /**
     * TEMP_CFG_REG_A bits
     */
    typedef enum {
        // 0x01-0x20 reserved

        LSM303AGR_TEMP_CFG_REG_A_TEMP_EN0         = 0x40,
        LSM303AGR_TEMP_CFG_REG_A_TEMP_EN1         = 0x80,
        _LSM303AGR_TEMP_CFG_REG_A_TEMP_EN_MASK    = 3,
        _LSM303AGR_TEMP_CFG_REG_A_TEMP_EN_SHIFT   = 6,
    } LSM303AGR_TEMP_CFG_REG_A_BITS_T;

    /**
     * TEMP_CFG_REG_A_TEMP_EN values
     */
    typedef enum {
        LSM303AGR_TEMP_EN_OFF                     = 0,
        LSM303AGR_TEMP_EN_ON                      = 3,
    } LSM303AGR_TEMP_EN_T;

    /**
     * CTRL_REG1_A bits
     */
    typedef enum {
        LSM303AGR_CTRL_REG1_A_XEN                 = 0x01, // axis enables
        LSM303AGR_CTRL_REG1_A_YEN                 = 0x02,
        LSM303AGR_CTRL_REG1_A_ZEN                 = 0x04,
        LSM303AGR_CTRL_REG1_A_LPEN                = 0x08, // low power enable

        LSM303AGR_CTRL_REG1_A_ODR0                = 0x10, // output data rate
        LSM303AGR_CTRL_REG1_A_ODR1                = 0x20,
        LSM303AGR_CTRL_REG1_A_ODR2                = 0x40,
        LSM303AGR_CTRL_REG1_A_ODR3                = 0x80,
        _LSM303AGR_CTRL_REG1_A_ODR_MASK           = 15,
        _LSM303AGR_CTRL_REG1_A_ODR_SHIFT          = 4,
    } LSM303AGR_CTRL_REG1_A_BITS_T;

    /**
     * CTRL_REG1_A_ODR values (and power mode)
     */
    typedef enum {
        LSM303AGR_A_ODR_POWER_DOWN                = 0,
        LSM303AGR_A_ODR_1HZ                       = 1, // 1Hz, HR/Normal/LP
        LSM303AGR_A_ODR_10HZ                      = 2,
        LSM303AGR_A_ODR_25HZ                      = 3,
        LSM303AGR_A_ODR_50HZ                      = 4,
        LSM303AGR_A_ODR_100HZ                     = 5,
        LSM303AGR_A_ODR_200HZ                     = 6,
        LSM303AGR_A_ODR_400HZ                     = 7,
        LSM303AGR_A_ODR_1_620KHZ                  = 8, // 1.620kHz, low power
        LSM303AGR_A_ODR_1_344KHZ                  = 9, // 1.344kHZ
                                                       // HR/Normal,
                                                       // 5.376kHZ low
                                                       // power
    } LSM303AGR_A_ODR_T;

    /**
     * CTRL_REG2_A bits
     */
    typedef enum {
        LSM303AGR_CTRL_REG2_A_HPIS1               = 0x01,
        LSM303AGR_CTRL_REG2_A_HPIS2               = 0x02,
        LSM303AGR_CTRL_REG2_A_HPCLICK             = 0x04,
        LSM303AGR_CTRL_REG2_A_FDS                 = 0x08,
        LSM303AGR_CTRL_REG2_A_HPCF1               = 0x10,
        LSM303AGR_CTRL_REG2_A_HPCF2               = 0x20,

        LSM303AGR_CTRL_REG2_A_HPM0                = 0x40,
        LSM303AGR_CTRL_REG2_A_HPM1                = 0x80,
        _LSM303AGR_CTRL_REG2_A_HPM_MASK           = 3,
        _LSM303AGR_CTRL_REG2_A_HPM_SHIFT          = 6,
    } LSM303AGR_CTRL_REG2_A_BITS_T;

    /**
     * CTRL_REG2_A_HPM values
     */
    typedef enum {
        LSM303AGR_A_HPM_NORMAL                    = 0,
        LSM303AGR_A_HPM_REF                       = 1,
        LSM303AGR_A_HPM_NORMAL2                   = 2,
        LSM303AGR_A_HPM_AUTORESET                 = 3,
    } LSM303AGR_A_HPM_T;

    /**
     * CTRL_REG3_A bits
     */
    typedef enum {
        // 0x01 reserved

        LSM303AGR_CTRL_REG3_A_I1_OVERRUN          = 0x02,
        LSM303AGR_CTRL_REG3_A_I1_WTM              = 0x04,
        LSM303AGR_CTRL_REG3_A_I1_DRDY2            = 0x08,
        LSM303AGR_CTRL_REG3_A_I1_DRDY1            = 0x10,
        LSM303AGR_CTRL_REG3_A_I1_AOI2             = 0x20,
        LSM303AGR_CTRL_REG3_A_I1_AOI1             = 0x40,
        LSM303AGR_CTRL_REG3_A_I1_CLICK            = 0x80,
    } LSM303AGR_CTRL_REG3_A_BITS_T;

    /**
     * CTRL_REG4_A bits
     */
    typedef enum {
        LSM303AGR_CTRL_REG4_A_SPI_ENABLE          = 0x01, // 3-wire
                                                          // spi (no
                                                          // supported!)
        LSM303AGR_CTRL_REG4_A_ST0                 = 0x02,
        LSM303AGR_CTRL_REG4_A_ST1                 = 0x04,
        _LSM303AGR_CTRL_REG4_A_ST_MASK            = 3,
        _LSM303AGR_CTRL_REG4_A_ST_SHIFT           = 1,

        LSM303AGR_CTRL_REG4_A_HR                  = 0x08, // high res

        LSM303AGR_CTRL_REG4_A_FS0                 = 0x10, // FS selection
        LSM303AGR_CTRL_REG4_A_FS1                 = 0x20,
        _LSM303AGR_CTRL_REG4_A_FS_MASK            = 3,
        _LSM303AGR_CTRL_REG4_A_FS_SHIFT           = 4,

        LSM303AGR_CTRL_REG4_A_BLE                 = 0x40,
        LSM303AGR_CTRL_REG4_A_BDU                 = 0x80, // block update
    } LSM303AGR_CTRL_REG4_A_BITS_T;

    /**
     * CTRL_REG4_A_ST values (self-test)
     */
    typedef enum {
        LSM303AGR_A_ST_NORMAL                     = 0,
        LSM303AGR_A_ST_0                          = 1,
        LSM303AGR_A_ST_1                          = 2,
    } LSM303AGR_A_ST_T;

    /**
     * CTRL_REG4_A_FS values (full-scale)
     */
    typedef enum {
        LSM303AGR_A_FS_2G                         = 0, // 2G
        LSM303AGR_A_FS_4G                         = 1,
        LSM303AGR_A_FS_8G                         = 2,
        LSM303AGR_A_FS_16G                        = 3, // 16G
    } LSM303AGR_A_FS_T;

    /**
     * CTRL_REG5_A bits
     */
    typedef enum {
        LSM303AGR_CTRL_REG5_A_D4D_INT2            = 0x01,
        LSM303AGR_CTRL_REG5_A_LIR_INT2            = 0x02,
        LSM303AGR_CTRL_REG5_A_D4D_INT1            = 0x04,
        LSM303AGR_CTRL_REG5_A_LIR_INT1            = 0x08,

        // 0x10-0x20 reserved

        LSM303AGR_CTRL_REG5_A_FIFO_EN             = 0x40,
        LSM303AGR_CTRL_REG5_A_BOOT                = 0x80,
    } LSM303AGR_CTRL_REG5_A_BITS_T;

    /**
     * CTRL_REG6_A bits
     */
    typedef enum {
        // 0x01 reserved

        LSM303AGR_CTRL_REG6_A_H_LACTIVE           = 0x02,

        // 0x04 reserved

        LSM303AGR_CTRL_REG6_A_P2_ACT              = 0x08,
        LSM303AGR_CTRL_REG6_A_BOOT_I2             = 0x10,
        LSM303AGR_CTRL_REG6_A_I2_INT2             = 0x20,
        LSM303AGR_CTRL_REG6_A_I2_INT1             = 0x40,
        LSM303AGR_CTRL_REG6_A_I2_CLICK_EN         = 0x80,
    } LSM303AGR_CTRL_REG6_A_BITS_T;

    /**
     * STATUS_REG_A bits
     */
    typedef enum {
        LSM303AGR_STATUS_REG_A_XDA                = 0x01,
        LSM303AGR_STATUS_REG_A_YDA                = 0x02,
        LSM303AGR_STATUS_REG_A_ZDA                = 0x04,
        LSM303AGR_STATUS_REG_A_ZYXDA              = 0x08,
        LSM303AGR_STATUS_REG_A_XOR                = 0x10,
        LSM303AGR_STATUS_REG_A_YOR                = 0x20,
        LSM303AGR_STATUS_REG_A_ZOR                = 0x40,
        LSM303AGR_STATUS_REG_A_ZYXOR              = 0x80,
    } LSM303AGR_STATUS_REG_A_BITS_T;

    /**
     * FIFO_CTRL_REG_A bits
     */
    typedef enum {
        LSM303AGR_FIFO_CTRL_REG_A_FTH0            = 0x01, // fifo threshold
        LSM303AGR_FIFO_CTRL_REG_A_FTH1            = 0x02,
        LSM303AGR_FIFO_CTRL_REG_A_FTH2            = 0x04,
        LSM303AGR_FIFO_CTRL_REG_A_FTH3            = 0x08,
        LSM303AGR_FIFO_CTRL_REG_A_FTH4            = 0x10,
        _LSM303AGR_FIFO_CTRL_REG_A_FTH_MASK       = 31,
        _LSM303AGR_FIFO_CTRL_REG_A_FTH_SHIF       = 0,

        LSM303AGR_FIFO_CTRL_REG_A_TR              = 0x20, // trigger select

        LSM303AGR_FIFO_CTRL_REG_A_FM0             = 0x40, // fifo mode
        LSM303AGR_FIFO_CTRL_REG_A_FM1             = 0x80,
        _LSM303AGR_FIFO_CTRL_REG_A_FM_MASK        = 3,
        _LSM303AGR_FIFO_CTRL_REG_A_FM_SHIFT       = 6,
    } LSM303AGR_FIFO_CTRL_REG_A_BITS_T;

    /**
     * FIFO_CTRL_REG_A_FM values (fifo mode)
     */
    typedef enum {
        LSM303AGR_A_FM_BYPASS                     = 0,
        LSM303AGR_A_FM_FIFO                       = 1,
        LSM303AGR_A_FM_STREAM                     = 2,
        LSM303AGR_A_FM_STREAM_TO_FIFO             = 3,
    } LSM303AGR_A_FM_T;

    /**
     * FIFO_SRC_REG_A bits
     */
    typedef enum {
        LSM303AGR_FIFO_SRC_REG_A_FSS0             = 0x01, // fifo unread samples
        LSM303AGR_FIFO_SRC_REG_A_FSS1             = 0x02,
        LSM303AGR_FIFO_SRC_REG_A_FSS2             = 0x04,
        LSM303AGR_FIFO_SRC_REG_A_FSS3             = 0x08,
        LSM303AGR_FIFO_SRC_REG_A_FSS4             = 0x10,
        _LSM303AGR_FIFO_SRC_REG_A_FSS_MASK        = 31,
        _LSM303AGR_FIFO_SRC_REG_A_FSS_SHIFT       = 0,

        LSM303AGR_FIFO_SRC_REG_A_EMPTY            = 0x20,
        LSM303AGR_FIFO_SRC_REG_A_OVRN_FIFO        = 0x40,
        LSM303AGR_FIFO_SRC_REG_A_WTM              = 0x80,
    } LSM303AGR_FIFO_SRC_REG_A_BITS_T;

    /**
     * INT_CFG_A bits.  This definition is used for INT1 and INT2.
     */
    typedef enum {
        LSM303AGR_INT_CFG_A_XLIE                  = 0x01,
        LSM303AGR_INT_CFG_A_XHIE                  = 0x02,
        LSM303AGR_INT_CFG_A_YLIE                  = 0x04,
        LSM303AGR_INT_CFG_A_YHIE                  = 0x08,
        LSM303AGR_INT_CFG_A_ZLIE                  = 0x10,
        LSM303AGR_INT_CFG_A_ZHIE                  = 0x20,
        LSM303AGR_INT_CFG_A_6D                    = 0x40,
        LSM303AGR_INT_CFG_A_AOI                   = 0x80,
    } LSM303AGR_INT_CFG_A_BITS_T;

    /**
     * INT_SRC_A bits.  This definition is used for INT1 and INT2.
     */
    typedef enum {
        LSM303AGR_INT_SRC_A_XL                    = 0x01,
        LSM303AGR_INT_SRC_A_XH                    = 0x02,
        LSM303AGR_INT_SRC_A_YL                    = 0x04,
        LSM303AGR_INT_SRC_A_YH                    = 0x08,
        LSM303AGR_INT_SRC_A_ZL                    = 0x10,
        LSM303AGR_INT_SRC_A_ZH                    = 0x20,
        LSM303AGR_INT_SRC_A_IA                    = 0x40,

        // 0x80 reserved
    } LSM303AGR_INT_SRC_A_BITS_T;

    /**
     * INT_THS_A bits.  This definition is used for INT1 and INT2.
     */
    typedef enum {
        LSM303AGR_INT_THS0                       = 0x01,
        LSM303AGR_INT_THS1                       = 0x02,
        LSM303AGR_INT_THS2                       = 0x04,
        LSM303AGR_INT_THS3                       = 0x08,
        LSM303AGR_INT_THS4                       = 0x10,
        LSM303AGR_INT_THS5                       = 0x20,
        LSM303AGR_INT_THS6                       = 0x40,
        _LSM303AGR_INT_THS_MASK                  = 127,
        _LSM303AGR_INT_THS_SHIFT                 = 0,

        // 0x80 reserved
    } LSM303AGR_INT_THS_BITS_T;

    /**
     * INT_DUR_A bits.  This definition is used for INT1 and INT2.
     */
    typedef enum {
        LSM303AGR_INT_DUR0                       = 0x01,
        LSM303AGR_INT_DUR1                       = 0x02,
        LSM303AGR_INT_DUR2                       = 0x04,
        LSM303AGR_INT_DUR3                       = 0x08,
        LSM303AGR_INT_DUR4                       = 0x10,
        LSM303AGR_INT_DUR5                       = 0x20,
        LSM303AGR_INT_DUR6                       = 0x40,
        _LSM303AGR_INT_DUR_MASK                  = 127,
        _LSM303AGR_INT_DUR_SHIFT                 = 0,

        // 0x80 reserved
    } LSM303AGR_INT_DUR_BITS_T;

    /**
     * CLICK_CFG_A bits
     */
    typedef enum {
        LSM303AGR_CLICK_CFG_A_XS                 = 0x01,
        LSM303AGR_CLICK_CFG_A_XD                 = 0x02,
        LSM303AGR_CLICK_CFG_A_YS                 = 0x04,
        LSM303AGR_CLICK_CFG_A_YD                 = 0x08,
        LSM303AGR_CLICK_CFG_A_ZS                 = 0x10,
        LSM303AGR_CLICK_CFG_A_ZD                 = 0x20,

        // 0x40-0x80 reserved
    } LSM303AGR_CLICK_CFG_A_BITS_T;

    /**
     * CLICK_SRC_A bits
     */
    typedef enum {
        LSM303AGR_CLICK_SRC_A_X                  = 0x01,
        LSM303AGR_CLICK_SRC_A_Y                  = 0x02,
        LSM303AGR_CLICK_SRC_A_Z                  = 0x04,
        LSM303AGR_CLICK_SRC_A_SIGN               = 0x08,
        LSM303AGR_CLICK_SRC_A_SCLICK             = 0x10,
        LSM303AGR_CLICK_SRC_A_DCLICK             = 0x20,
        LSM303AGR_CLICK_SRC_A_IA                 = 0x40,

        // 0x80 reserved
    } LSM303AGR_CLICK_SRC_A_BITS_T;

    /**
     * CLICK_THS_A bits
     */
    typedef enum {
        LSM303AGR_CLICK_A_THS0                   = 0x01,
        LSM303AGR_CLICK_A_THS1                   = 0x02,
        LSM303AGR_CLICK_A_THS2                   = 0x04,
        LSM303AGR_CLICK_A_THS3                   = 0x08,
        LSM303AGR_CLICK_A_THS4                   = 0x10,
        LSM303AGR_CLICK_A_THS5                   = 0x20,
        LSM303AGR_CLICK_A_THS6                   = 0x40,
        _LSM303AGR_CLICK_A_THS_MASK              = 127,
        _LSM303AGR_CLICK_A_THS_SHIFT             = 0,

        // 0x80 reserved
    } LSM303AGR_CLICK_THS_A_BITS_T;

    /**
     * TIME_LIMIT_A bits
     */
    typedef enum {
        LSM303AGR_TIME_LIMIT_A_TLI0              = 0x01,
        LSM303AGR_TIME_LIMIT_A_TLI1              = 0x02,
        LSM303AGR_TIME_LIMIT_A_TLI2              = 0x04,
        LSM303AGR_TIME_LIMIT_A_TLI3              = 0x08,
        LSM303AGR_TIME_LIMIT_A_TLI4              = 0x10,
        LSM303AGR_TIME_LIMIT_A_TLI5              = 0x20,
        LSM303AGR_TIME_LIMIT_A_TLI6              = 0x40,
        _LSM303AGR_TIME_LIMIT_A_TLI_MASK         = 127,
        _LSM303AGR_TIME_LIMIT_A_TLI_SHIFT        = 0,

        // 0x80 reserved
    } LSM303AGR_TIME_LIMIT_A_BITS_T;

    // Magnetometer registers

    /**
     * CFG_REG_A_M bits
     */
    typedef enum {
        LSM303AGR_CFG_REG_A_M_MD0                = 0x01, // mode select
        LSM303AGR_CFG_REG_A_M_MD1                = 0x02,
        _LSM303AGR_CFG_REG_A_M_MD_MASK           = 3,
        _LSM303AGR_CFG_REG_A_M_MD_SHIFT          = 0,

        LSM303AGR_CFG_REG_A_M_ODR0               = 0x04, // output data rate
        LSM303AGR_CFG_REG_A_M_ODR1               = 0x08,
        _LSM303AGR_CFG_REG_A_M_ODR_MASK          = 3,
        _LSM303AGR_CFG_REG_A_M_ODR_SHIFT         = 2,

        LSM303AGR_CFG_REG_A_M_LP                 = 0x10, // low power/hi res
        LSM303AGR_CFG_REG_A_M_SOFT_RESET         = 0x20,
        LSM303AGR_CFG_REG_A_M_REBOOT             = 0x40,
        LSM303AGR_CFG_REG_A_M_COMP_TEMP_EN       = 0x80, // temp compensation
    } LSM303AGR_CFG_REG_A_M_BITS_T;

    /**
     * CFG_REG_A_M_MD values
     */
    typedef enum {
        LSM303AGR_CFG_A_M_MD_CONTINUOUS          = 0,
        LSM303AGR_CFG_A_M_MD_SINGLE              = 1,
        LSM303AGR_CFG_A_M_MD_IDLE                = 2,
    } LSM303AGR_CFG_A_M_MD_T;

    /**
     * CFG_REG_A_M_ODR values
     */
    typedef enum {
        LSM303AGR_CFG_A_M_ODR_10HZ               = 0,
        LSM303AGR_CFG_A_M_ODR_20HZ               = 1,
        LSM303AGR_CFG_A_M_ODR_50HZ               = 2,
        LSM303AGR_CFG_A_M_ODR_100HZ              = 3,
    } LSM303AGR_CFG_A_M_ODR_T;

    /**
     * CFG_REG_B_M bits
     */
    typedef enum {
        LSM303AGR_CFG_REG_B_M_LPF                = 0x01,
        LSM303AGR_CFG_REG_B_M_OFF_CANC           = 0x02,
        LSM303AGR_CFG_REG_B_M_SET_FREQ           = 0x04,
        LSM303AGR_CFG_REG_B_M_INT_ON_DATA_OFF    = 0x08,
        LSM303AGR_CFG_REG_B_M_OFF_CANC_ONE_SHOT  = 0x10,

        // 0x20-0x80 reserved
    } LSM303AGR_CFG_REG_B_M_BITS_T;

    /**
     * CFG_REG_C_M bits
     */
    typedef enum {
        LSM303AGR_CFG_REG_C_M_INT_MAG             = 0x01,
        LSM303AGR_CFG_REG_C_M_SELF_TEST           = 0x02,

        // 0x04 reserved and MUST be 0 for correct operation

        LSM303AGR_CFG_REG_C_M_BLE                 = 0x08,
        LSM303AGR_CFG_REG_C_M_BDU                 = 0x10,
        LSM303AGR_CFG_REG_C_M_I2C_DIS             = 0x20, // don't do it...
        LSM303AGR_CFG_REG_C_M_INT_MAG_PIN         = 0x40,

        // 0x80 reserved
    } LSM303AGR_CFG_REG_C_M_BITS_T;

    /**
     * INT_CTRL_REG_M bits
     */
    typedef enum {
        LSM303AGR_INT_CTRL_REG_M_IEN              = 0x01,
        LSM303AGR_INT_CTRL_REG_M_IEL              = 0x02,
        LSM303AGR_INT_CTRL_REG_M_IEA              = 0x04,

        // 0x08-0x10 reserved and MUST be 0 for correct operation

        LSM303AGR_INT_CTRL_REG_M_ZIEN             = 0x20,
        LSM303AGR_INT_CTRL_REG_M_YIEN             = 0x40,
        LSM303AGR_INT_CTRL_REG_M_XIEN             = 0x80,
    } LSM303AGR_INT_CTRL_REG_M_BITS_T;

    /**
     * INT_SRC_REG_M bits
     */
    typedef enum {
        LSM303AGR_INT_SRC_REG_M_MROI              = 0x02,
        LSM303AGR_INT_SRC_REG_M_N_TH_S_Z          = 0x04,
        LSM303AGR_INT_SRC_REG_M_N_TH_S_Y          = 0x08,
        LSM303AGR_INT_SRC_REG_M_N_TH_S_X          = 0x10,
        LSM303AGR_INT_SRC_REG_M_P_TH_S_Z          = 0x20,
        LSM303AGR_INT_SRC_REG_M_P_TH_S_Y          = 0x40,
        LSM303AGR_INT_SRC_REG_M_P_TH_S_X          = 0x80,
    } LSM303AGR_INT_SRC_REG_M_BITS_T;

    /**
     * STATUS_REG_M bits
     */
    typedef enum {
        LSM303AGR_STATUS_REG_M_XDA                = 0x01,
        LSM303AGR_STATUS_REG_M_YDA                = 0x02,
        LSM303AGR_STATUS_REG_M_ZDA                = 0x04,
        LSM303AGR_STATUS_REG_M_ZYXDA              = 0x08,
        LSM303AGR_STATUS_REG_M_XOR                = 0x10,
        LSM303AGR_STATUS_REG_M_YOR                = 0x20,
        LSM303AGR_STATUS_REG_M_ZOR                = 0x40,
        LSM303AGR_STATUS_REG_M_ZYXOR              = 0x80,
    } LSM303AGR_STATUS_REG_M_BITS_T;

    // The following enums are used for driver functionality.

    /**
     * INTERRUPT_PINS_T values.  There are 3 interrupt pins available;
     * two for the accelerometer, and one for the magnetometer.
     */
    typedef enum {
        LSM303AGR_INTERRUPT_ACC_1                 = 0, // ACC intr 1
        LSM303AGR_INTERRUPT_ACC_2                 = 1, // ACC intr 2
        LSM303AGR_INTERRUPT_MAG                   = 2, // MAG intr
    } LSM303AGR_INTERRUPT_PINS_T;

    /**
     * LSM303AGR_POWER_MODE_T values.  These set the basic operating
     * power modes to one of low power, normal, and high resolution.
     */
    typedef enum {
        LSM303AGR_POWER_LOW_POWER                 = 0,
        LSM303AGR_POWER_NORMAL                    = 1,
        LSM303AGR_POWER_HIGH_RESOLUTION           = 2,
    } LSM303AGR_POWER_MODE_T;


#ifdef __cplusplus
}
#endif
