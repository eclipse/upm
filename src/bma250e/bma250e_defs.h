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

#define BMA250E_DEFAULT_I2C_BUS 0
#define BMA250E_DEFAULT_SPI_BUS 0
#define BMA250E_DEFAULT_ADDR 0x18

// special reset byte
#define BMA250E_RESET_BYTE 0xb6

    // NOTE: Reserved registers must not be written into.  Reading
    // from them may return indeterminate values.  Registers
    // containing reserved bitfields must be written as 0.  Reading
    // reserved bitfields may return indeterminate values.

    /**
     * BMA250E registers
     */
    typedef enum  {
        BMA250E_REG_CHIP_ID                      = 0x00,

        // 0x01 reserved

        BMA250E_REG_ACCD_X_LSB                   = 0x02,
        BMA250E_REG_ACCD_X_MSB                   = 0x03,
        BMA250E_REG_ACCD_Y_LSB                   = 0x04,
        BMA250E_REG_ACCD_Y_MSB                   = 0x05,
        BMA250E_REG_ACCD_Z_LSB                   = 0x06,
        BMA250E_REG_ACCD_Z_MSB                   = 0x07,

        BMA250E_REG_TEMP                         = 0x08,

        BMA250E_REG_INT_STATUS_0                 = 0x09,
        BMA250E_REG_INT_STATUS_1                 = 0x0a,
        BMA250E_REG_INT_STATUS_2                 = 0x0b,
        BMA250E_REG_INT_STATUS_3                 = 0x0c,

        // 0x0d reserved

        BMA250E_REG_FIFO_STATUS                  = 0x0e,

        BMA250E_REG_PMU_RANGE                    = 0x0f,
        BMA250E_REG_PMU_BW                       = 0x10,
        BMA250E_REG_PMU_LPW                      = 0x11,
        BMA250E_REG_PMU_LOW_POWER                = 0x12,

        BMA250E_REG_ACC_HBW                      = 0x13,

        BMA250E_REG_SOFTRESET                    = 0x14,

        // 0x15 reserved

        BMA250E_REG_INT_EN_0                     = 0x16,
        BMA250E_REG_INT_EN_1                     = 0x17,
        BMA250E_REG_INT_EN_2                     = 0x18,

        BMA250E_REG_INT_MAP_0                    = 0x19,
        BMA250E_REG_INT_MAP_1                    = 0x1a,
        BMA250E_REG_INT_MAP_2                    = 0x1b,

        // 0x1c-0x1d reserved

        BMA250E_REG_INT_SRC                      = 0x1e,

        // 0x1f reserved

        BMA250E_REG_INT_OUT_CTRL                 = 0x20,
        BMA250E_REG_INT_RST_LATCH                = 0x21,

        BMA250E_REG_INT_0                        = 0x22,
        BMA250E_REG_INT_1                        = 0x23,
        BMA250E_REG_INT_2                        = 0x24,
        BMA250E_REG_INT_3                        = 0x25,
        BMA250E_REG_INT_4                        = 0x26,
        BMA250E_REG_INT_5                        = 0x27,
        BMA250E_REG_INT_6                        = 0x28,
        BMA250E_REG_INT_7                        = 0x29,
        BMA250E_REG_INT_8                        = 0x2a,
        BMA250E_REG_INT_9                        = 0x2b,
        BMA250E_REG_INT_A                        = 0x2c,
        BMA250E_REG_INT_B                        = 0x2d,
        BMA250E_REG_INT_C                        = 0x2e,
        BMA250E_REG_INT_D                        = 0x2f,

        BMA250E_REG_FIFO_CONFIG_0                = 0x30,

        // 0x31 reserved

        BMA250E_REG_PMU_SELFTEST                 = 0x32,

        BMA250E_REG_TRIM_NVM_CTRL                = 0x33,

        BMA250E_REG_SPI3_WDT                     = 0x34,

        // 0x35 reserved

        BMA250E_REG_OFC_CTRL                     = 0x36,
        BMA250E_REG_OFC_SETTING                  = 0x37,

        BMA250E_REG_OFC_OFFSET_X                 = 0x38,
        BMA250E_REG_OFC_OFFSET_Y                 = 0x39,
        BMA250E_REG_OFC_OFFSET_Z                 = 0x3a,

        BMA250E_REG_TRIM_GP0                     = 0x3b,
        BMA250E_REG_TRIM_GP1                     = 0x3c,

        // 0x3d reserved

        BMA250E_REG_FIFO_CONFIG_1                = 0x3e,
        BMA250E_REG_FIFO_DATA                    = 0x3f

    } BMA250E_REGS_T;

    /**
     * REG_ACCD_*_LSB bits - handle X, Y, and Z LSB regs, for 10 bit
     * resolution
     */
    typedef enum {
        BMA250E_ACCD10_LSB_NEW_DATA              = 0x01, // data
                                                         // updated
                                                         // since last
                                                         // read

        // 0x02-0x20 reserved

        BMA250E_ACCD10_LSB0                      = 0x40, // lower 2
                                                         // bits of
                                                         // LSB data
        BMA250E_ACCD10_LSB1                      = 0x80,
        _BMA250E_ACCD10_LSB_MASK                 = 3,
        _BMA250E_ACCD10_LSB_SHIFT                = 6
    } BMA250E_ACCD10_LSB_BITS_T;

    /**
     * REG_ACCD_*_LSB bits - handle X, Y, and Z LSB regs, for 12 bit
     * resolution
     */
    typedef enum {
        BMA250E_ACCD12_LSB_NEW_DATA              = 0x01, // data
                                                         // updated
                                                         // since last
                                                         // read

        // 0x02-0x08 reserved

        BMA250E_ACCD12_LSB0                      = 0x10, // lower 4
                                                         // bits of
                                                         // LSB data
        BMA250E_ACCD12_LSB1                      = 0x20,
        BMA250E_ACCD12_LSB2                      = 0x40,
        BMA250E_ACCD12_LSB3                      = 0x80,
        _BMA250E_ACCD12_LSB_MASK                 = 15,
        _BMA250E_ACCD12_LSB_SHIFT                = 4
    } BMA250E_ACCD12_LSB_BITS_T;

    /**
     * REG_INT_STATUS_0 bits
     */
    typedef enum {
        BMA250E_INT_STATUS_0_LOW                 = 0x01,
        BMA250E_INT_STATUS_0_HIGH                = 0x02,
        BMA250E_INT_STATUS_0_SLOPE               = 0x04,
        BMA250E_INT_STATUS_0_SLO_NOT_MOT         = 0x08,
        BMA250E_INT_STATUS_0_D_TAP               = 0x10,
        BMA250E_INT_STATUS_0_S_TAP               = 0x20,
        BMA250E_INT_STATUS_0_ORIENT              = 0x40,
        BMA250E_INT_STATUS_0_FLAT                = 0x80
    } BMA250E_INT_STATUS_0_BITS_T;

    /**
     * REG_INT_STATUS_1 bits
     */
    typedef enum {
        _BMA250E_INT_STATUS_1_RESERVED_BITS      = 0x0f | 0x10,
        // 0x01-0x10 reserved
        BMA250E_INT_STATUS_1_FIFO_FULL           = 0x20,
        BMA250E_INT_STATUS_1_FIFO_WM             = 0x40,
        BMA250E_INT_STATUS_1_DATA                = 0x80 // data ready int
    } BMA250E_INT_STATUS_1_BITS_T;

    /**
     * REG_INT_STATUS_2 bits
     */
    typedef enum {
        BMA250E_INT_STATUS_2_SLOPE_FIRST_X       = 0x01,
        BMA250E_INT_STATUS_2_SLOPE_FIRST_Y       = 0x02,
        BMA250E_INT_STATUS_2_SLOPE_FIRST_Z       = 0x04,
        BMA250E_INT_STATUS_2_SLOPE_SIGN          = 0x08,
        BMA250E_INT_STATUS_2_TAP_FIRST_X         = 0x10,
        BMA250E_INT_STATUS_2_TAP_FIRST_Y         = 0x20,
        BMA250E_INT_STATUS_2_TAP_FIRST_Z         = 0x40,
        BMA250E_INT_STATUS_2_TAP_SIGN            = 0x80
    } BMA250E_INT_STATUS_2_BITS_T;

    /**
     * REG_INT_STATUS_3 bits
     */
    typedef enum {
        BMA250E_INT_STATUS_3_HIGH_FIRST_X        = 0x01,
        BMA250E_INT_STATUS_3_HIGH_FIRST_Y        = 0x02,
        BMA250E_INT_STATUS_3_HIGH_FIRST_Z        = 0x04,
        BMA250E_INT_STATUS_3_HIGH_SIGN           = 0x08,

        BMA250E_INT_STATUS_3_ORIENT0             = 0x10,
        BMA250E_INT_STATUS_3_ORIENT1             = 0x20,
        BMA250E_INT_STATUS_3_ORIENT2             = 0x40,
        _BMA250E_INT_STATUS_3_ORIENT_MASK        = 7,
        _BMA250E_INT_STATUS_3_ORIENT_SHIFT       = 4,

        BMA250E_INT_STATUS_3_FLAT                = 0x80
    } INT_STATUS_3_BITS_T;

    /**
     * INT_STATUS_3_ORIENT values
     */
    typedef enum {
        BMA250E_ORIENT_POTRAIT_UPRIGHT           = 0,
        BMA250E_ORIENT_POTRAIT_UPSIDE_DOWN       = 1,
        BMA250E_ORIENT_LANDSCAPE_LEFT            = 2,
        BMA250E_ORIENT_LANDSCAPE_RIGHT           = 3,
    } BMA250E_ORIENT_T;

    /**
     * REG_FIFO_STATUS bits
     */
    typedef enum {
        BMA250E_FIFO_STATUS_FRAME_COUNTER0       = 0x01,
        BMA250E_FIFO_STATUS_FRAME_COUNTER1       = 0x02,
        BMA250E_FIFO_STATUS_FRAME_COUNTER2       = 0x04,
        BMA250E_FIFO_STATUS_FRAME_COUNTER3       = 0x08,
        BMA250E_FIFO_STATUS_FRAME_COUNTER4       = 0x10,
        BMA250E_FIFO_STATUS_FRAME_COUNTER5       = 0x20,
        BMA250E_FIFO_STATUS_FRAME_COUNTER6       = 0x40,
        _BMA250E_FIFO_STATUS_FRAME_COUNTER_MASK  = 127,
        _BMA250E_FIFO_STATUS_FRAME_COUNTER_SHIFT = 0,

        BMA250E_FIFO_STATUS_FIFO_OVERRUN         = 0x80
    } BMA250E_FIFO_STATUS_BITS_T;

    /**
     * REG_PMU_RANGE bits
     */
    typedef enum {
        BMA250E_PMU_RANGE0                       = 0x01,
        BMA250E_PMU_RANGE1                       = 0x02,
        BMA250E_PMU_RANGE2                       = 0x04,
        BMA250E_PMU_RANGE3                       = 0x08,
        _BMA250E_PMU_RANGE_MASK                  = 15,
        _BMA250E_PMU_RANGE_SHIFT                 = 0

        // 0x10-0x80 reserved
    } BMA250E_PMU_RANGE_BITS_T;

    /**
     * PMU_RANGE (accelerometer g-range) values
     */
    typedef enum {
        BMA250E_RANGE_2G                         = 3,
        BMA250E_RANGE_4G                         = 5,
        BMA250E_RANGE_8G                         = 8,
        BMA250E_RANGE_16G                        = 12
    } BMA250E_RANGE_T;

    /**
     * REG_PMU_BW bits
     */
    typedef enum {
        BMA250E_PMU_BW0                          = 0x01,
        BMA250E_PMU_BW1                          = 0x02,
        BMA250E_PMU_BW2                          = 0x04,
        BMA250E_PMU_BW3                          = 0x08,
        BMA250E_PMU_BW4                          = 0x10,
        _BMA250E_PMU_BW_MASK                     = 31,
        _BMA250E_PMU_BW_SHIFT                    = 0

        // 0x20-0x80 reserved
    } BMA250E_PMU_BW_BITS_T;

    /**
     * PMU_BW (accelerometer filter bandwidth) values
     */
    typedef enum {
        BMA250E_BW_7_81                          = 8, // 7.81 Hz
        BMA250E_BW_15_63                         = 9,
        BMA250E_BW_31_25                         = 10,
        BMA250E_BW_62_5                          = 11,
        BMA250E_BW_125                           = 12,
        BMA250E_BW_250                           = 13,
        BMA250E_BW_500                           = 14,
        BMA250E_BW_1000                          = 15
    } BMA250E_BW_T;

    /**
     * REG_PMU_LPW bits
     */
    typedef enum {
        // 0x01 reserved
        _BMA250E_PMU_LPW_RESERVED_MASK           = 0x01,

        BMA250E_PMU_LPW_SLEEP_DUR0               = 0x02, // sleep dur
                                                         // in low
                                                         // power mode
        BMA250E_PMU_LPW_SLEEP_DUR1               = 0x04,
        BMA250E_PMU_LPW_SLEEP_DUR2               = 0x08,
        BMA250E_PMU_LPW_SLEEP_DUR3               = 0x10,
        _BMA250E_PMU_LPW_SLEEP_MASK              = 15,
        _BMA250E_PMU_LPW_SLEEP_SHIFT             = 1,

        // These are separate bits, deep_suspend, lowpower_en and
        // suspend (and if all 0, normal).  Since only specific
        // combinations are allowed, we will treat this as a 3 bit
        // bitfield called POWER_MODE.
        BMA250E_PMU_LPW_POWER_MODE0              = 0x20, // deep_suspend
        BMA250E_PMU_LPW_POWER_MODE1              = 0x40, // lowpower_en
        BMA250E_PMU_LPW_POWER_MODE2              = 0x80, // suspend
        _BMA250E_PMU_LPW_POWER_MODE_MASK         = 7,
        _BMA250E_PMU_LPW_POWER_MODE_SHIFT        = 5
    } BMA250E_PMU_LPW_BITS_T;

    /**
     * SLEEP_DUR values
     */
    typedef enum {
        BMA250E_SLEEP_DUR_0_5                    = 0, // 0.5ms
        BMA250E_SLEEP_DUR_1                      = 6,
        BMA250E_SLEEP_DUR_2                      = 7,
        BMA250E_SLEEP_DUR_4                      = 8,
        BMA250E_SLEEP_DUR_6                      = 9,
        BMA250E_SLEEP_DUR_10                     = 10,
        BMA250E_SLEEP_DUR_25                     = 11,
        BMA250E_SLEEP_DUR_50                     = 12,
        BMA250E_SLEEP_DUR_100                    = 13,
        BMA250E_SLEEP_DUR_500                    = 14,
        BMA250E_SLEEP_DUR_1000                   = 15
    } BMA250E_SLEEP_DUR_T;

    /**
     * POWER_MODE values
     */
    typedef enum {
        BMA250E_POWER_MODE_NORMAL                = 0,
        BMA250E_POWER_MODE_DEEP_SUSPEND          = 1,
        BMA250E_POWER_MODE_LOW_POWER             = 2,
        BMA250E_POWER_MODE_SUSPEND               = 4
    } BMA250E_POWER_MODE_T;

    /**
     * REG_PMU_LOW_POWER bits
     */
    typedef enum {
        _BMA250E_LOW_POWER_RESERVED_BITS         = 0x0f | 0x10 | 0x80,

        // 0x01-0x10 reserved
        BMA250E_LOW_POWER_SLEEPTIMER_MODE        = 0x20,
        BMA250E_LOW_POWER_LOWPOWER_MODE          = 0x40  // LPM1 or
                                                         // LPM2
                                                         // mode. see
                                                         // DS.
        // 0x80 reserved
    } BMA250E_LOW_POWER_BITS_T;

    /**
     * REG_ACC_HBW bits
     */
    typedef enum {
        _BMA250E_ACC_HBW_RESERVED_BITS           = 0x0f | 0x10 | 0x20,

        // 0x01-0x20 reserved
        BMA250E_ACC_HBW_SHADOW_DIS               = 0x40,
        BMA250E_ACC_HBW_DATA_HIGH_BW             = 0x80
    } BMA250E_ACC_HBW_BITS_T;

    /**
     * REG_INT_EN_0 bits
     */
    typedef enum {
        _BMA250E_INT_EN_0_RESERVED_BITS          = 0x08,

        BMA250E_INT_EN_0_SLOPE_EN_X              = 0x01,
        BMA250E_INT_EN_0_SLOPE_EN_Y              = 0x02,
        BMA250E_INT_EN_0_SLOPE_EN_Z              = 0x04,

        // 0x08 reserved

        BMA250E_INT_EN_0_D_TAP_EN                = 0x10,
        BMA250E_INT_EN_0_S_TAP_EN                = 0x20,
        BMA250E_INT_EN_0_ORIENT_EN               = 0x40,
        BMA250E_INT_EN_0_FLAT_EN                 = 0x80
    } BMA250E_INT_EN_0_BITS_T;

    /**
     * REG_INT_EN_1 bits
     */
    typedef enum {
        _BMA250E_INT_EN_1_RESERVED_BITS          = 0x80,

        BMA250E_INT_EN_1_HIGH_EN_X               = 0x01,
        BMA250E_INT_EN_1_HIGH_EN_Y               = 0x02,
        BMA250E_INT_EN_1_HIGH_EN_Z               = 0x04,
        BMA250E_INT_EN_1_LOW_EN                  = 0x08,
        BMA250E_INT_EN_1_DATA_EN                 = 0x10,
        BMA250E_INT_EN_1_INT_FFULL_EN            = 0x20, // fifo full
        BMA250E_INT_EN_1_INT_FWM_EN              = 0x40  // fifo watermark

        // 0x80 reserved
    } BMA250E_INT_EN_1_BITS_T;

    /**
     * REG_INT_EN_2 bits
     */
    typedef enum {
        _BMA250E_INT_EN_2_RESERVED_BITS          = 0xf0,

        BMA250E_INT_EN_2_SLO_NO_MOT_EN_X         = 0x01,
        BMA250E_INT_EN_2_SLO_NO_MOT_EN_Y         = 0x02,
        BMA250E_INT_EN_2_SLO_NO_MOT_EN_Z         = 0x04,
        BMA250E_INT_EN_2_SLO_NO_MOT_SEL          = 0x08

        // 0x10-0x80 reserved
    } BMA250E_INT_EN_2_BITS_T;

    /**
     * REG_INT_MAP_0 bits
     */
    typedef enum {
        BMA250E_INT_MAP_0_INT1_LOW               = 0x01,
        BMA250E_INT_MAP_0_INT1_HIGH              = 0x02,
        BMA250E_INT_MAP_0_INT1_SLOPE             = 0x04,
        BMA250E_INT_MAP_0_INT1_SLO_NO_MOT        = 0x08,
        BMA250E_INT_MAP_0_INT1_D_TAP             = 0x10,
        BMA250E_INT_MAP_0_INT1_S_TAP             = 0x20,
        BMA250E_INT_MAP_0_INT1_ORIENT            = 0x40,
        BMA250E_INT_MAP_0_INT1_FLAT              = 0x80
    } BMA250E_INT_MAP_0_BITS_T;

    /**
     * REG_INT_MAP_1 bits
     */
    typedef enum {
        _BMA250E_INT_MAP_1_INT1_RESERVED_BITS    = 0x08 | 0x10,

        BMA250E_INT_MAP_1_INT1_DATA              = 0x01,
        BMA250E_INT_MAP_1_INT1_FWM               = 0x02,
        BMA250E_INT_MAP_1_INT1_FFULL             = 0x04,

        // 0x08-0x10 reserved

        BMA250E_INT_MAP_1_INT2_FFULL             = 0x20,
        BMA250E_INT_MAP_1_INT2_FWM               = 0x40,
        BMA250E_INT_MAP_1_INT2_DATA              = 0x80
    } BMA250E_INT_MAP_1_BITS_T;

    /**
     * REG_INT_MAP_2 bits
     */
    typedef enum {
        BMA250E_INT_MAP_2_INT2_LOW               = 0x01,
        BMA250E_INT_MAP_2_INT2_HIGH              = 0x02,
        BMA250E_INT_MAP_2_INT2_SLOPE             = 0x04,
        BMA250E_INT_MAP_2_INT2_SLO_NO_MOT        = 0x08,
        BMA250E_INT_MAP_2_INT2_D_TAP             = 0x10,
        BMA250E_INT_MAP_2_INT2_S_TAP             = 0x20,
        BMA250E_INT_MAP_2_INT2_ORIENT            = 0x40,
        BMA250E_INT_MAP_2_INT2_FLAT              = 0x80
    } BMA250E_INT_MAP_2_BITS_T;

    /**
     * REG_INT_SRC bits
     */
    typedef enum {
        _BMA250E_INT_SRC_RESERVED_BITS           = 0x40 | 0x80,

        BMA250E_INT_SRC_LOW                      = 0x01,
        BMA250E_INT_SRC_HIGH                     = 0x02,
        BMA250E_INT_SRC_SLO_NO_MOT               = 0x04,
        BMA250E_INT_SRC_SLOPE                    = 0x08,
        BMA250E_INT_SRC_TAP                      = 0x10,
        BMA250E_INT_SRC_DATA                     = 0x20

        // 0x40-0x80 reserved
    } BMA250E_INT_SRC_BITS_T;

    /**
     * REG_INT_OUT_CTRL bits
     */
    typedef enum {
        _BMA250E_INT_OUT_CTRL_INT1_RESERVED_BITS = 0xf0,

        BMA250E_INT_OUT_CTRL_INT1_LVL            = 0x01, // level or edge
        BMA250E_INT_OUT_CTRL_INT1_OD             = 0x02, // push-pull
                                                         // or open
                                                         // drain
        BMA250E_INT_OUT_CTRL_INT2_LVL            = 0x04,
        BMA250E_INT_OUT_CTRL_INT2_OD             = 0x08

        // 0x10-0x80 reserved
    } BMA250E_INT_OUT_CTRL_BITS_T;

    /**
     * REG_INT_RST_LATCH bits
     */
    typedef enum {
        _BMA250E_INT_RST_LATCH_RESERVED_BITS     = 0x10 | 0x20 | 0x40,

        BMA250E_INT_RST_LATCH0                   = 0x01,
        BMA250E_INT_RST_LATCH1                   = 0x02,
        BMA250E_INT_RST_LATCH2                   = 0x04,
        BMA250E_INT_RST_LATCH3                   = 0x08,
        _BMA250E_INT_RST_LATCH_MASK              = 15,
        _BMA250E_INT_RST_LATCH_SHIFT             = 0,

        // 0x10-0x40 reserved

        BMA250E_INT_RST_LATCH_RESET_INT          = 0x80
    } BMA250E_INT_RST_LATCH_BITS_T;

    /**
     * RST_LATCH values
     */
    typedef enum {
        BMA250E_RST_LATCH_NON_LATCHED            = 0,
        BMA250E_RST_LATCH_TEMPORARY_250MS        = 1,
        BMA250E_RST_LATCH_TEMPORARY_500MS        = 2,
        BMA250E_RST_LATCH_TEMPORARY_1S           = 3,
        BMA250E_RST_LATCH_TEMPORARY_2S           = 4,
        BMA250E_RST_LATCH_TEMPORARY_4S           = 5,
        BMA250E_RST_LATCH_TEMPORARY_8S           = 6,
        BMA250E_RST_LATCH_LATCHED                = 7,

        // 8 == non latched

        BMA250E_RST_LATCH_TEMPORARY_250US        = 9,
        BMA250E_RST_LATCH_TEMPORARY_500US        = 10,
        BMA250E_RST_LATCH_TEMPORARY_1MS          = 11,
        BMA250E_RST_LATCH_TEMPORARY_12_5MS       = 12,
        BMA250E_RST_LATCH_TEMPORARY_25MS         = 13,
        BMA250E_RST_LATCH_TEMPORARY_50MS         = 14

        // 15 == latched
    } BMA250E_RST_LATCH_T;

    /**
     * REG_INT_2 bits
     */
    typedef enum {
        BMA250E_INT_2_LOW_HY0                    = 0x01,
        BMA250E_INT_2_LOW_HY1                    = 0x02,
        _BMA250E_INT_2_LOW_HY_MASK               = 3,
        _BMA250E_INT_2_LOW_HY_SHIFT              = 0,

        BMA250E_INT_2_LOW_MODE                   = 0x04,

        // 0x08-0x20 reserved

        BMA250E_INT_2_HIGH_HY0                   = 0x40,
        BMA250E_INT_2_HIGH_HY1                   = 0x80,
        _BMA250E_INT_2_HIGH_HY_MASK              = 3,
        _BMA250E_INT_2_HIGH_HY_SHIFT             = 6
    } BMA250E_INT_2_BITS_T;

    /**
     * REG_INT_5 bits
     */
    typedef enum {
        BMA250E_INT_5_SLOPE_DUR0                 = 0x01,
        BMA250E_INT_5_SLOPE_DUR1                 = 0x02,
        _BMA250E_INT_5_SLOPE_DUR_MASK            = 3,
        _BMA250E_INT_5_SLOPE_DUR_SHIFT           = 0,

        BMA250E_INT_5_SLO_NO_MOT_DUR0            = 0x04,
        BMA250E_INT_5_SLO_NO_MOT_DUR1            = 0x08,
        BMA250E_INT_5_SLO_NO_MOT_DUR2            = 0x10,
        BMA250E_INT_5_SLO_NO_MOT_DUR3            = 0x20,
        BMA250E_INT_5_SLO_NO_MOT_DUR4            = 0x40,
        BMA250E_INT_5_SLO_NO_MOT_DUR5            = 0x80,
        _BMA250E_INT_5_SLO_NO_MOT_DUR_MASK       = 63,
        _BMA250E_INT_5_SLO_NO_MOT_DUR_SHIFT      = 2
    } BMA250E_INT_5_BITS_T;

    /**
     * REG_INT_8 bits
     */
    typedef enum {
        BMA250E_INT_8_TAP_DUR0                   = 0x01,
        BMA250E_INT_8_TAP_DUR1                   = 0x02,
        BMA250E_INT_8_TAP_DUR2                   = 0x04,
        _BMA250E_INT_8_TAP_DUR_MASK              = 7,
        _BMA250E_INT_8_TAP_DUR_SHIFT             = 0,

        // 0x08-0x20 reserved

        BMA250E_INT_8_TAP_SHOCK                  = 0x40,
        BMA250E_INT_8_TAP_QUIET                  = 0x80
    } BMA250E_INT_8_BITS_T;

    /**
     * REG_INT_9 bits
     */
    typedef enum {
        BMA250E_INT_9_TAP_TH0                    = 0x01,
        BMA250E_INT_9_TAP_TH1                    = 0x02,
        BMA250E_INT_9_TAP_TH2                    = 0x04,
        BMA250E_INT_9_TAP_TH3                    = 0x08,
        BMA250E_INT_9_TAP_TH4                    = 0x10,
        _BMA250E_INT_5_TAP_TH_MASK               = 31,
        _BMA250E_INT_5_TAP_TH_SHIFT              = 0,

        // 0x20 reserved

        BMA250E_INT_9_TAP_SAMP0                  = 0x40,
        BMA250E_INT_9_TAP_SAMP1                  = 0x80,
        BMA250E_INT_9_TAP_SAMP1_MASK             = 3,
        BMA250E_INT_9_TAP_SAMP1_SHIFT            = 6
    } BMA250E_INT_9_BITS_T;

    /**
     * REG_INT_A bits
     */
    typedef enum {
        BMA250E_INT_A_ORIENT_MODE0               = 0x01,
        BMA250E_INT_A_ORIENT_MODE1               = 0x02,
        _BMA250E_INT_A_ORIENT_MODE_MASK          = 3,
        _BMA250E_INT_A_ORIENT_MODE_SHIFT         = 0,

        BMA250E_INT_A_ORIENT_BLOCKING0           = 0x04,
        BMA250E_INT_A_ORIENT_BLOCKING1           = 0x08,
        _BMA250E_INT_A_ORIENT_BLOCKING_MASK      = 3,
        _BMA250E_INT_A_ORIENT_BLOCKING_SHIFT     = 2,

        BMA250E_INT_A_ORIENT_HYST0               = 0x10,
        BMA250E_INT_A_ORIENT_HYST1               = 0x20,
        BMA250E_INT_A_ORIENT_HYST2               = 0x40,
        _BMA250E_INT_A_ORIENT_HYST_MASK          = 7,
        _BMA250E_INT_A_ORIENT_HYST_SHIFT         = 4

        // 0x80 reserved
    } BMA250E_INT_A_BITS_T;

    /**
     * INT_A_ORIENT_MODE values
     */
    typedef enum {
        BMA250E_ORIENT_MODE_SYMETRICAL           = 0,
        BMA250E_ORIENT_MODE_HIGH_ASYMETRICAL     = 1,
        BMA250E_ORIENT_MODE_LOW_ASYMETRICAL      = 2
    } BMA250E_ORIENT_MODE_T;

    /**
     * INT_A_ORIENT_BLOCKING values
     */
    typedef enum {
        BMA250E_ORIENT_BLOCKING_NONE             = 0,
        BMA250E_ORIENT_BLOCKING_THETA_ACC_1_5G   = 1,
        BMA250E_ORIENT_BLOCKING_THETA_ACC_0_2G_1_5G = 2,
        BMA250E_ORIENT_BLOCKING_THETA_ACC_0_4G_1_5G = 3
    } BMA250E_ORIENT_BLOCKING_T;

    /**
     * REG_INT_B bits
     */
    typedef enum {
        BMA250E_INT_B_ORIENT_THETA0              = 0x01,
        BMA250E_INT_B_ORIENT_THETA1              = 0x02,
        BMA250E_INT_B_ORIENT_THETA2              = 0x04,
        BMA250E_INT_B_ORIENT_THETA3              = 0x08,
        BMA250E_INT_B_ORIENT_THETA4              = 0x10,
        BMA250E_INT_B_ORIENT_THETA5              = 0x20,
        _BMA250E_INT_B_ORIENT_THETA_MASK         = 63,
        _BMA250E_INT_B_ORIENT_THETA_SHIFT        = 0,

        BMA250E_INT_B_ORIENT_UD_EN               = 0x40
        // 0x80 reserved
    } BMA250E_INT_B_BITS_T;

    /**
     * REG_INT_C bits
     */
    typedef enum {
        BMA250E_INT_B_FLAT_THETA0               = 0x01,
        BMA250E_INT_B_FLAT_THETA1               = 0x02,
        BMA250E_INT_B_FLAT_THETA2               = 0x04,
        BMA250E_INT_B_FLAT_THETA3               = 0x08,
        BMA250E_INT_B_FLAT_THETA4               = 0x10,
        BMA250E_INT_B_FLAT_THETA5               = 0x20,
        _BMA250E_INT_B_FLAT_THETA_MASK          = 63,
        _BMA250E_INT_B_FLAT_THETA_SHIFT         = 0,

        // 0x40-0x80 reserved
    } BMA250E_INT_C_BITS_T;

    /**
     * REG_INT_D bits
     */
    typedef enum {
        BMA250E_INT_D_FLAT_HY0                  = 0x01,
        BMA250E_INT_D_FLAT_HY1                  = 0x02,
        BMA250E_INT_D_FLAT_HY2                  = 0x04,
        _BMA250E_INT_B_FLAT_HY_MASK             = 7,
        _BMA250E_INT_B_FLAT_HY_SHIFT            = 0,

        // 0x08 reserved

        BMA250E_INT_D_FLAT_HOLD_TIME0           = 0x10,
        BMA250E_INT_D_FLAT_HOLD_TIME1           = 0x20,
        _BMA250E_INT_B_FLAT_HOLD_TIME_MASK      = 3,
        _BMA250E_INT_B_FLAT_HOLD_TIME_SHIFT     = 4

        // 0x40-0x80 reserved
    } BMA250E_INT_D_BITS_T;

    /**
     * REG_FIFO_CONFIG_0 bits
     */
    typedef enum {
        _BMA250E_FIFO_CONFIG_0_RESERVED_BITS    = 0x80 | 0x40,

        BMA250E_FIFO_CONFIG_0_WATER_MARK0       = 0x01,
        BMA250E_FIFO_CONFIG_0_WATER_MARK1       = 0x02,
        BMA250E_FIFO_CONFIG_0_WATER_MARK2       = 0x04,
        BMA250E_FIFO_CONFIG_0_WATER_MARK3       = 0x08,
        BMA250E_FIFO_CONFIG_0_WATER_MARK4       = 0x10,
        BMA250E_FIFO_CONFIG_0_WATER_MARK5       = 0x20,
        _BMA250E_FIFO_CONFIG_0_WATER_MARK_MASK  = 63,
        _BMA250E_FIFO_CONFIG_0_WATER_MARK_SHIFT = 0
    } BMA250E_FIFO_CONFIG_0_BITS_T;

    /**
     * REG_PMU_SELFTTEST bits
     */
    typedef enum {
        BMA250E_PMU_SELFTTEST_AXIS0             = 0x01,
        BMA250E_PMU_SELFTTEST_AXIS1             = 0x02,
        _BMA250E_PMU_SELFTTEST_AXIS_MASK        = 3,
        _BMA250E_PMU_SELFTTEST_AXIS_SHIFT       = 0,

        BMA250E_PMU_SELFTTEST_SIGN              = 0x04,

        // 0x08 reserved

        BMA250E_PMU_SELFTTEST_AMP               = 0x10,

        // 0x20-0x80 reserved
    } BMA250E_PMU_SELFTTEST_BITS_T;

    /**
     * PMU_SELFTTEST_AXIS values
     */
    typedef enum {
        BMA250E_SELFTTEST_AXIS_NONE             = 0,
        BMA250E_SELFTTEST_AXIS_X                = 1,
        BMA250E_SELFTTEST_AXIS_Y                = 2,
        BMA250E_SELFTTEST_AXIS_Z                = 3,
    } BMA250E_SELFTTEST_AXIS_T;

    /**
     * REG_TRIM_NVM_CTRL bits
     */
    typedef enum {
        BMA250E_TRIM_NVM_CTRL_NVM_PROG_MODE     = 0x01,
        BMA250E_TRIM_NVM_CTRL_NVM_PROG_TRIG     = 0x02,
        BMA250E_TRIM_NVM_CTRL_NVM_PROG_RDY      = 0x04,
        BMA250E_TRIM_NVM_CTRL_NVM_PROG_LOAD     = 0x08,

        BMA250E_TRIM_NVM_CTRL_NVM_REMAIN0       = 0x10,
        BMA250E_TRIM_NVM_CTRL_NVM_REMAIN1       = 0x20,
        BMA250E_TRIM_NVM_CTRL_NVM_REMAIN2       = 0x40,
        BMA250E_TRIM_NVM_CTRL_NVM_REMAIN3       = 0x80,
        _BMA250E_TRIM_NVM_CTRL_NVM_REMAIN_MASK  = 15,
        _BMA250E_TRIM_NVM_CTRL_NVM_REMAIN_SHIFT = 4
    } BMA250E_TRIM_NVM_CTRL_BITS_T;

    /**
     * REG_SPI3_WDT bits
     */
    typedef enum {
        _BMA250E_SPI3_WDT_RESERVED_BITS         = 0xf0 | 0x08,

        BMA250E_SPI3_WDT_SPI3                   = 0x01, // 3-wire SPI
                                                        // - NOT
                                                        // SUPPORTED

        BMA250E_SPI3_WDT_I2C_WDT_SEL            = 0x02,
        BMA250E_SPI3_WDT_I2C_WDT_EN             = 0x04

        // 0x08-0x80 reserved
    } BMA250E_SPI3_WDT_BITS_T;

    /**
     * REG_OFC_CTRL bits
     */
    typedef enum {
        BMA250E_OFC_CTRL_HP_X_EN                = 0x01,
        BMA250E_OFC_CTRL_HP_Y_EN                = 0x02,
        BMA250E_OFC_CTRL_HP_Z_EN                = 0x04,

        // 0x08 reserved

        BMA250E_OFC_CTRL_CAL_RDY                = 0x10,

        BMA250E_OFC_CTRL_CAL_TRIGGER0           = 0x20,
        BMA250E_OFC_CTRL_CAL_TRIGGER1           = 0x40,
        _BMA250E_OFC_CTRL_CAL_TRIGGER_MASK      = 3,
        _BMA250E_OFC_CTRL_CAL_TRIGGER_SHIFT     = 5,

        BMA250E_OFC_CTRL_OFFSET_RESET           = 0x80

    } BMA250E_OFC_CTRL_BITS_T;

    /**
     * OFC_CTRL_CAL_TRIGGER values
     */
    typedef enum {
        BMA250E_CAL_TRIGGER_NONE                = 0,
        BMA250E_CAL_TRIGGER_X                   = 1,
        BMA250E_CAL_TRIGGER_Y                   = 2,
        BMA250E_CAL_TRIGGER_Z                   = 3
    } BMA250E_CAL_TRIGGER_T;

    /**
     * REG_OFC_SETTING bits
     */
    typedef enum {
        BMA250E_OFC_SETTING_CUT_OFF             = 0x01,

        BMA250E_OFC_SETTING_OFFSET_TARGET_X0    = 0x02,
        BMA250E_OFC_SETTING_OFFSET_TARGET_X1    = 0x04,
        _BMA250E_OFC_SETTING_OFFSET_TARGET_X_MASK = 3,
        _BMA250E_OFC_SETTING_OFFSET_TARGET_X_SHIFT = 1,

        BMA250E_OFC_SETTING_OFFSET_TARGET_Y0    = 0x08,
        BMA250E_OFC_SETTING_OFFSET_TARGET_Y1    = 0x10,
        _BMA250E_OFC_SETTING_OFFSET_TARGET_Y_MASK = 3,
        _BMA250E_OFC_SETTING_OFFSET_TARGET_Y_SHIFT = 3,

        BMA250E_OFC_SETTING_OFFSET_TARGET_Z0    = 0x20,
        BMA250E_OFC_SETTING_OFFSET_TARGET_Z1    = 0x40,
        _BMA250E_OFC_SETTING_OFFSET_TARGET_Z_MASK = 3,
        _BMA250E_OFC_SETTING_OFFSET_TARGET_Z_SHIFT = 5

        // 0x80 reserved
    } BMA250E_OFC_SETTING_BITS_T;

    /**
     * OFC_SETTING_OFFSET_TARGET (for X, Y and Z axis) values
     */
    typedef enum {
        BMA250E_OFFSET_TARGET_0G                = 0,
        BMA250E_OFFSET_TARGET_PLUS_1G           = 1,
        BMA250E_OFFSET_TARGET_MINUS_1G          = 2,
        // 3 == 0G
    } BMA250E_OFFSET_TARGET_T;

    /**
     * REG_FIFO_CONFIG_1 bits
     */
    typedef enum {
        BMA250E_FIFO_CONFIG_1_FIFO_DATA_SEL0     = 0x01,
        BMA250E_FIFO_CONFIG_1_FIFO_DATA_SEL1     = 0x02,
        _BMA250E_FIFO_CONFIG_1_FIFO_DATA_SEL     = 3,
        _BMA250E_FIFO_CONFIG_1_FIFO_DATA_SHIFT   = 0,

        // 0x04-0x20 reserved

        BMA250E_FIFO_CONFIG_1_FIFO_MODE0         = 0x40,
        BMA250E_FIFO_CONFIG_1_FIFO_MODE1         = 0x80,
        _BMA250E_FIFO_CONFIG_1_FIFO_MODE_MASK    = 3,
        _BMA250E_FIFO_CONFIG_1_FIFO_MODE_SHIFT   = 5
    } BMA250E_FIFO_CONFIG_1_BITS_T;

    /**
     * FIFO_DATA_SEL values
     */
    typedef enum {
        BMA250E_FIFO_DATA_SEL_XYZ               = 0,
        BMA250E_FIFO_DATA_SEL_X                 = 1,
        BMA250E_FIFO_DATA_SEL_Y                 = 2,
        BMA250E_FIFO_DATA_SEL_Z                 = 3
    } BMA250E_FIFO_DATA_SEL_T;

    /**
     * FIFO_MODE values
     */
    typedef enum {
        BMA250E_FIFO_MODE_BYPASS                = 0,
        BMA250E_FIFO_MODE_FIFO                  = 1,
        BMA250E_FIFO_MODE_STREAM                = 2

        // 3 == reserved (execute self-destruct :)
    } BMA250E_FIFO_MODE_T;

    // interrupt selection for installISR() and uninstallISR()
    typedef enum {
        BMA250E_INTERRUPT_INT1,
        BMA250E_INTERRUPT_INT2
    } BMA250E_INTERRUPT_PINS_T;

    // Different variants of this chip support different resolutions.
    // The 0xf9 variant supports 10b, while the 0xfa variant (bmx055)
    // supports 12 bits.
    typedef enum {
        BMA250E_RESOLUTION_10BITS,
        BMA250E_RESOLUTION_12BITS
    } BMA250E_RESOLUTION_T;

#ifdef __cplusplus
}
#endif
