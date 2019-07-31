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

#define BMM150_DEFAULT_I2C_BUS 0
#define BMM150_DEFAULT_SPI_BUS 0
#define BMM150_DEFAULT_ADDR 0x10

#define BMM150_DEFAULT_CHIPID 0x32

    // NOTE: Reserved registers must not be written into.  Reading
    // from them may return indeterminate values.  Registers
    // containing reserved bitfields must be written as 0.  Reading
    // reserved bitfields may return indeterminate values.

    /**
     * BMM150 registers
     */
    typedef enum {
        BMM150_REG_CHIP_ID                      = 0x40,

        // 0x41 reserved

        BMM150_REG_MAG_X_LSB                    = 0x42,
        BMM150_REG_MAG_X_MSB                    = 0x43,
        BMM150_REG_MAG_Y_LSB                    = 0x44,
        BMM150_REG_MAG_Y_MSB                    = 0x45,
        BMM150_REG_MAG_Z_LSB                    = 0x46,
        BMM150_REG_MAG_Z_MSB                    = 0x47,

        BMM150_REG_RHALL_LSB                    = 0x48,
        BMM150_REG_RHALL_MSB                    = 0x49,

        BMM150_REG_INT_STATUS                   = 0x4a,

        BMM150_REG_POWER_CTRL                   = 0x4b,

        BMM150_REG_OPMODE                       = 0x4c,

        BMM150_REG_INT_EN                       = 0x4d,
        BMM150_REG_INT_CONFIG                   = 0x4e,

        BMM150_REG_LOW_THRES                    = 0x4f,
        BMM150_REG_HIGH_THRES                   = 0x50,

        BMM150_REG_REP_XY                       = 0x51,
        BMM150_REG_REP_Z                        = 0x52,

        // 0x53-0x71 reserved (mostly)

        // TRIM registers from Bosch BMM050 driver
        BMM150_REG_TRIM_DIG_X1                  = 0x5d,
        BMM150_REG_TRIM_DIG_Y1                  = 0x5e,

        BMM150_REG_TRIM_DIG_Z4_LSB              = 0x62,
        BMM150_REG_TRIM_DIG_Z4_MSB              = 0x63,
        BMM150_REG_TRIM_DIG_X2                  = 0x64,
        BMM150_REG_TRIM_DIG_Y2                  = 0x65,

        BMM150_REG_TRIM_DIG_Z2_LSB              = 0x68,
        BMM150_REG_TRIM_DIG_Z2_MSB              = 0x69,
        BMM150_REG_TRIM_DIG_Z1_LSB              = 0x6a,
        BMM150_REG_TRIM_DIG_Z1_MSB              = 0x6b,
        BMM150_REG_TRIM_DIG_XYZ1_LSB            = 0x6c,
        BMM150_REG_TRIM_DIG_XYZ1_MSB            = 0x6d,
        BMM150_REG_TRIM_DIG_Z3_LSB              = 0x6e,
        BMM150_REG_TRIM_DIG_Z3_MSB              = 0x6f,
        BMM150_REG_TRIM_DIG_XY2                 = 0x70,
        BMM150_REG_TRIM_DIG_XY1                 = 0x71
    } BMM150_REGS_T;

    /**
     * REG_MAG_XY_LSB bits (for X and Y mag data LSB's only)
     */
    typedef enum {
        _BMM150_MAG_XY_LSB_RESERVED_BITS        = 0x02 | 0x04,

        BMM150_MAG_XY_LSB_SELFTEST_XY           = 0x01,

        BMM150_MAG_XY_LSB_LSB0                  = 0x08,
        BMM150_MAG_XY_LSB_LSB1                  = 0x10,
        BMM150_MAG_XY_LSB_LSB2                  = 0x20,
        BMM150_MAG_XY_LSB_LSB3                  = 0x40,
        BMM150_MAG_XY_LSB_LSB4                  = 0x80,
        _BMM150_MAG_XY_LSB_LSB_MASK             = 31,
        _BMM150_MAG_XY_LSB_LSB_SHIFT            = 3
    } BMM150_MAG_XY_LSB_BITS_T;

    /**
     * REG_MAG_Z_LSB bits (for Z LSB only)
     */
    typedef enum {
        BMM150_MAG_Z_LSB_SELFTEST_Z             = 0x01,

        BMM150_MAG_Z_LSB_LSB0                   = 0x02,
        BMM150_MAG_Z_LSB_LSB1                   = 0x04,
        BMM150_MAG_Z_LSB_LSB2                   = 0x08,
        BMM150_MAG_Z_LSB_LSB3                   = 0x10,
        BMM150_MAG_Z_LSB_LSB4                   = 0x20,
        BMM150_MAG_Z_LSB_LSB5                   = 0x40,
        BMM150_MAG_Z_LSB_LSB6                   = 0x80,
        _BMM150_MAG_Z_LSB_LSB_MASK              = 127,
        _BMM150_MAG_Z_LSB_LSB_SHIFT             = 1
    } MAG_Z_LSB_BITS_T;

    /**
     * REG_MAG_RHALL_LSB bits (for RHALL LSB only)
     */
    typedef enum {
        _BMM150_MAG_RHALL_LSB_RESERVED_BITS     = 0x02,

        BMM150_MAG_RHALL_LSB_DATA_READY_STATUS  = 0x01,

        BMM150_MAG_RHALL_LSB_LSB0               = 0x04,
        BMM150_MAG_RHALL_LSB_LSB1               = 0x08,
        BMM150_MAG_RHALL_LSB_LSB2               = 0x10,
        BMM150_MAG_RHALL_LSB_LSB3               = 0x20,
        BMM150_MAG_RHALL_LSB_LSB4               = 0x40,
        BMM150_MAG_RHALL_LSB_LSB5               = 0x80,
        _BMM150_MAG_RHALL_LSB_LSB_MASK          = 63,
        _BMM150_MAG_RHALL_LSB_LSB_SHIFT         = 2
    } BMM150_MAG_RHALL_LSB_BITS_T;

    /**
     * REG_INT_STATUS bits
     */
    typedef enum {
        BMM150_INT_STATUS_LOW_INT_X             = 0x01,
        BMM150_INT_STATUS_LOW_INT_Y             = 0x02,
        BMM150_INT_STATUS_LOW_INT_Z             = 0x04,
        BMM150_INT_STATUS_HIGH_INT_X            = 0x08,
        BMM150_INT_STATUS_HIGH_INT_Y            = 0x10,
        BMM150_INT_STATUS_HIGH_INT_Z            = 0x20,
        BMM150_INT_STATUS_OVERFLOW              = 0x40,
        BMM150_INT_STATUS_DATA_OVERRUN          = 0x80
    } BMM150_INT_STATUS_BITS_T;

    /**
     * REG_POWER_CTRL bits
     */
    typedef enum {
        _BMM150_POWER_CTRL_RESERVED_BITS        = 0x40 | 0x20 | 0x10 | 0x08,

        BMM150_POWER_CTRL_POWER_CTRL_BIT        = 0x01,
        BMM150_POWER_CTRL_SOFT_RESET0           = 0x02,
        BMM150_POWER_CTRL_SPI3EN                = 0x04, // not supported

        BMM150_POWER_CTRL_SOFT_RESET1           = 0x80
    } POWER_CTRL_BITS_T;

    /**
     * REG_OPMODE bits
     */
    typedef enum {
        BMM150_OPMODE_SELFTTEST                 = 0x01,

        BMM150_OPMODE_OPERATION_MODE0           = 0x02,
        BMM150_OPMODE_OPERATION_MODE1           = 0x04,
        _BMM150_OPMODE_OPERATION_MODE_MASK      = 3,
        _BMM150_OPMODE_OPERATION_MODE_SHIFT     = 1,

        BMM150_OPMODE_DATA_RATE0                = 0x08,
        BMM150_OPMODE_DATA_RATE1                = 0x10,
        BMM150_OPMODE_DATA_RATE2                = 0x20,
        _BMM150_OPMODE_DATA_RATE_MASK           = 7,
        _BMM150_OPMODE_DATA_RATE_SHIFT          = 3,

        BMM150_OPMODE_ADV_SELFTEST0             = 0x40,
        BMM150_OPMODE_ADV_SELFTEST1             = 0x80,
        _BMM150_OPMODE_ADV_SELFTEST_MASK        = 3,
        _BMM150_OPMODE_ADV_SELFTEST_SHIFT       = 6
    } OPMODE_BITS_T;

    /**
     * OPMODE_OPERATION_MODE values
     */
    typedef enum {
        BMM150_OPERATION_MODE_NORMAL            = 0,
        BMM150_OPERATION_MODE_FORCED            = 1,
        BMM150_OPERATION_MODE_SLEEP             = 3
    } BMM150_OPERATION_MODE_T;

    /**
     * OPMODE_DATA_RATE values
     */
    typedef enum {
        BMM150_DATA_RATE_10HZ                   = 0,
        BMM150_DATA_RATE_2HZ                    = 1,
        BMM150_DATA_RATE_6HZ                    = 2,
        BMM150_DATA_RATE_8HZ                    = 3,
        BMM150_DATA_RATE_15HZ                   = 4,
        BMM150_DATA_RATE_20HZ                   = 5,
        BMM150_DATA_RATE_25HZ                   = 6,
        BMM150_DATA_RATE_30HZ                   = 7
    } BMM150_DATA_RATE_T;

    /**
     * REG_INT_EN bits
     */
    typedef enum {
        BMM150_INT_EN_LOW_INT_X_EN              = 0x01,
        BMM150_INT_EN_LOW_INT_Y_EN              = 0x02,
        BMM150_INT_EN_LOW_INT_Z_EN              = 0x04,
        BMM150_INT_EN_HIGH_INT_X_EN             = 0x08,
        BMM150_INT_EN_HIGH_INT_Y_EN             = 0x10,
        BMM150_INT_EN_HIGH_INT_Z_EN             = 0x20,
        BMM150_INT_EN_OVERFLOW_INT_EN           = 0x40,
        BMM150_INT_EN_DATA_OVERRUN_INT_EN       = 0x80
    } BMM150_INT_EN_T;

    /**
     * REG_INT_CONFIG bits
     */
    typedef enum {
        BMM150_INT_CONFIG_INT_POLARITY          = 0x01,
        BMM150_INT_CONFIG_INT_LATCH             = 0x02,
        BMM150_INT_CONFIG_DR_POLARITY           = 0x04,
        BMM150_INT_CONFIG_CHANNEL_X             = 0x08,
        BMM150_INT_CONFIG_CHANNEL_Y             = 0x10,
        BMM150_INT_CONFIG_CHANNEL_Z             = 0x20,
        BMM150_INT_CONFIG_INT_PIN_EN            = 0x40,
        BMM150_INT_CONFIG_DR_PIN_EN             = 0x80
    } BMM150_INT_CONFIG_T;

    /**
     * Interrupt selection for installISR() and uninstallISR()
     */
    typedef enum {
        BMM150_INTERRUPT_INT,
        BMM150_INTERRUPT_DR
    } BMM150_INTERRUPT_PINS_T;

    /**
     * Bosch recommended usage preset modes
     */
    typedef enum {
        BMM150_USAGE_LOW_POWER,
        BMM150_USAGE_REGULAR,
        BMM150_USAGE_ENHANCED_REGULAR,
        BMM150_USAGE_HIGH_ACCURACY
    } BMM150_USAGE_PRESETS_T;

#ifdef __cplusplus
}
#endif
