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

#define BMG160_DEFAULT_I2C_BUS 0
#define BMG160_DEFAULT_SPI_BUS 0
#define BMG160_DEFAULT_ADDR 0x68

// special reset byte
#define BMG160_RESET_BYTE 0xb6

#define BMG160_CHIPID 0x0f

    // NOTE: Reserved registers must not be written into.  Reading
    // from them may return indeterminate values.  Registers
    // containing reserved bitfields must be written as 0.  Reading
    // reserved bitfields may return indeterminate values.

    /**
     * BMG160 registers
     */
    typedef enum {
        BMG160_REG_CHIP_ID                      = 0x00,

        // 0x01 reserved

        BMG160_REG_RATE_X_LSB                   = 0x02,
        BMG160_REG_RATE_X_MSB                   = 0x03,
        BMG160_REG_RATE_Y_LSB                   = 0x04,
        BMG160_REG_RATE_Y_MSB                   = 0x05,
        BMG160_REG_RATE_Z_LSB                   = 0x06,
        BMG160_REG_RATE_Z_MSB                   = 0x07,

        BMG160_REG_TEMP                         = 0x08,

        BMG160_REG_INT_STATUS_0                 = 0x09,
        BMG160_REG_INT_STATUS_1                 = 0x0a,
        BMG160_REG_INT_STATUS_2                 = 0x0b,
        BMG160_REG_INT_STATUS_3                 = 0x0c,

        // 0x0d reserved

        BMG160_REG_FIFO_STATUS                  = 0x0e,

        BMG160_REG_GYR_RANGE                    = 0x0f,
        BMG160_REG_GYR_BW                       = 0x10,
        BMG160_REG_LPM1                         = 0x11,
        BMG160_REG_LPM2                         = 0x12,

        BMG160_REG_RATE_HBW                     = 0x13,

        BMG160_REG_SOFTRESET                    = 0x14,

        BMG160_REG_INT_EN_0                     = 0x15,
        BMG160_REG_INT_EN_1                     = 0x16,

        BMG160_REG_INT_MAP_0                    = 0x17,
        BMG160_REG_INT_MAP_1                    = 0x18,
        BMG160_REG_INT_MAP_2                    = 0x19,

        BMG160_REG_INT_1A                       = 0x1a,
        BMG160_REG_INT_1B                       = 0x1b,
        BMG160_REG_INT_1C                       = 0x1c,

        // 0x1d reserved

        BMG160_REG_INT_1E                       = 0x1e,

        // 0x1f-0x20 reserved

        BMG160_REG_INT_RST_LATCH                = 0x21,

        BMG160_REG_HIGH_TH_X                    = 0x22,
        BMG160_REG_HIGH_DUR_X                   = 0x23,
        BMG160_REG_HIGH_TH_Y                    = 0x24,
        BMG160_REG_HIGH_DUR_Y                   = 0x25,
        BMG160_REG_HIGH_TH_Z                    = 0x26,
        BMG160_REG_HIGH_DUR_Z                   = 0x27,

        // 0x28-0x30 reserved

        BMG160_REG_SOC                          = 0x31,
        BMG160_REG_A_FOC                        = 0x32,

        BMG160_REG_TRIM_NVM_CTRL                = 0x33,

        BMG160_REG_SPI3_WDT                     = 0x34,

        // 0x35 reserved

        BMG160_REG_OFC1                         = 0x36,
        BMG160_REG_OFC2                         = 0x37,
        BMG160_REG_OFC3                         = 0x38,
        BMG160_REG_OFC4                         = 0x39,

        BMG160_REG_TRIM_GP0                     = 0x3a,
        BMG160_REG_TRIM_GP1                     = 0x3b,

        BMG160_REG_BIST                         = 0x3c,

        BMG160_REG_FIFO_CONFIG_0                = 0x3d,
        BMG160_REG_FIFO_CONFIG_1                = 0x3e,

        BMG160_REG_FIFO_DATA                    = 0x3f

    } BMG160_REGS_T;

    /**
     * REG_INT_STATUS_0 bits
     */
    typedef enum {
        _BMG160_INT_STATUS_0_RESERVED_BITS      = 0xf0 | 0x08 | 0x01,

        BMG160_INT_STATUS_0_HIGH_INT            = 0x02,
        BMG160_INT_STATUS_0_ANY_INT             = 0x04
    } BMG160_INT_STATUS_0_BITS_T;

    /**
     * REG_INT_STATUS_1 bits
     */
    typedef enum {
        _BMG160_INT_STATUS_1_RESERVED_BITS      = 0x0f,

        BMG160_INT_STATUS_1_FIFO_INT            = 0x10,
        BMG160_INT_STATUS_1_FAST_OFFSET_INT     = 0x20,
        BMG160_INT_STATUS_1_AUTO_OFFSET_INT     = 0x40,
        BMG160_INT_STATUS_1_DATA_INT            = 0x80
    } BMG160_INT_STATUS_1_BITS_T;

    /**
     * REG_INT_STATUS_2 bits
     */
    typedef enum {
        _BMG160_INT_STATUS_2_RESERVED_BITS      = 0xf0,

        BMG160_INT_STATUS_2_ANY_FIRST_X         = 0x01,
        BMG160_INT_STATUS_2_ANY_FIRST_Y         = 0x02,
        BMG160_INT_STATUS_2_ANY_FIRST_Z         = 0x04,
        BMG160_INT_STATUS_2_ANY_SIGN            = 0x08
    } BMG160_INT_STATUS_2_BITS_T;

    /**
     * REG_INT_STATUS_3 bits
     */
    typedef enum {
        _BMG160_INT_STATUS_3_RESERVED_BITS      = 0xf0,

        BMG160_INT_STATUS_3_HIGH_FIRST_X        = 0x01,
        BMG160_INT_STATUS_3_HIGH_FIRST_Y        = 0x02,
        BMG160_INT_STATUS_3_HIGH_FIRST_Z        = 0x04,
        BMG160_INT_STATUS_3_HIGH_SIGN           = 0x08
    } BMG160_INT_STATUS_3_BITS_T;

    /**
     * REG_FIFO_STATUS bits
     */
    typedef enum {
        BMG160_FIFO_STATUS_FRAME_COUNTER0       = 0x01,
        BMG160_FIFO_STATUS_FRAME_COUNTER1       = 0x02,
        BMG160_FIFO_STATUS_FRAME_COUNTER2       = 0x04,
        BMG160_FIFO_STATUS_FRAME_COUNTER3       = 0x08,
        BMG160_FIFO_STATUS_FRAME_COUNTER4       = 0x10,
        BMG160_FIFO_STATUS_FRAME_COUNTER5       = 0x20,
        BMG160_FIFO_STATUS_FRAME_COUNTER6       = 0x40,
        _BMG160_FIFO_STATUS_FRAME_COUNTER_MASK  = 127,
        _BMG160_FIFO_STATUS_FRAME_COUNTER_SHIFT = 0,

        BMG160_FIFO_STATUS_FIFO_OVERRUN         = 0x80
    } BMG160_FIFO_STATUS_BITS_T;

    /**
     * REG_GYR_RANGE bits
     */
    typedef enum {
        _BMG160_GYR_RANGE_RESERVED_BITS         = 0x20 | 0x10 | 0x08,

        BMG160_GYR_RANGE0                       = 0x01,
        BMG160_GYR_RANGE1                       = 0x02,
        BMG160_GYR_RANGE2                       = 0x04,
        _BMG160_GYR_RANGE_MASK                  = 7,
        _BMG160_GYR_RANGE_SHIFT                 = 0,

        BMG160_GYR_RANGE_FIXED0                 = 0x40, // bits need
                                                        // hardcoding
                                                        // to 0b10
        BMG160_GYR_RANGE_FIXED1                 = 0x80, // for some
                                                        // odd
                                                        // reason...
        _BMG160_GYR_RANGE_FIXED_MASK            = 3,
        _BMG160_GYR_RANGE_FIXED_SHIFT           = 6,
        _BMG160_GYR_RANGE_FIXED_VALUE           = 2    // 0b10
    } BMG160_GYR_RANGE_BITS_T;

    /**
     * GYR_RANGE (gyroscope deg/s range) values
     */
    typedef enum {
        BMG160_RANGE_2000                       = 0, // degrees/sec
        BMG160_RANGE_1000                       = 1,
        BMG160_RANGE_500                        = 2,
        BMG160_RANGE_250                        = 3,
        BMG160_RANGE_125                        = 4
    } BMG160_RANGE_T;

    /**
     * REG_GYR_BW bits
     */
    typedef enum {
        _BMG160_GYR_BW_RESERVED_BITS            = 0xf0,

        BMG160_GYR_BW0                          = 0x01,
        BMG160_GYR_BW1                          = 0x02,
        BMG160_GYR_BW2                          = 0x04,
        BMG160_GYR_BW3                          = 0x08,
        _BMG160_GYR_BW_MASK                     = 15,
        _BMG160_GYR_BW_SHIFT                    = 0
    } BMG160_GYR_BW_BITS_T;

    /**
     * GYR_BW (gyroscope filter bandwidth) values
     */
    typedef enum {
        BMG160_BW_2000_UNFILTERED               = 0, // ODR/Filter BW
        BMG160_BW_2000_230                      = 1, // ODR 2000Hz,
                                                     // Filter BW
                                                     // 230Hz
        BMG160_BW_1000_116                      = 2,
        BMG160_BW_400_47                        = 3,
        BMG160_BW_200_23                        = 4,
        BMG160_BW_100_12                        = 5,
        BMG160_BW_200_64                        = 6,
        BMG160_BW_100_32                        = 7
    } BMG160_BW_T;

    /**
     * REG_LPM1 bits
     */
    typedef enum {
        // 0x01 reserved
        _BMG160_LPM1_RESERVED_BITS              = 0x40 | 0x10 | 0x01,

        BMG160_LPM1_SLEEP_DUR0                  = 0x02, // sleep dur
                                                        // in low
                                                        // power mode
        BMG160_LPM1_SLEEP_DUR1                  = 0x04,
        BMG160_LPM1_SLEEP_DUR2                  = 0x08,
        _BMG160_LPM1_SLEEP_MASK                 = 7,
        _BMG160_LPM1_SLEEP_SHIFT                = 1,

        // These are separate bits, deep_suspend and suspend (and if all
        // 0, normal).  Since only specific combinations are allowed, we
        // will treat this as a 3 bit bitfield called POWER_MODE.
        BMG160_LPM1_POWER_MODE0                 = 0x20, // deep_suspend
        BMG160_LPM1_POWER_MODE1                 = 0x40, // must always be 0!
        BMG160_LPM1_POWER_MODE2                 = 0x80, // suspend
        _BMG160_LPM1_POWER_MODE_MASK            = 7,
        _BMG160_LPM1_POWER_MODE_SHIFT           = 5
    } BMG160_LPM1_BITS_T;

    /**
     * SLEEP_DUR values
     */
    typedef enum {
        BMG160_SLEEP_DUR_2                      = 0, // 2ms
        BMG160_SLEEP_DUR_4                      = 1,
        BMG160_SLEEP_DUR_5                      = 2,
        BMG160_SLEEP_DUR_8                      = 3,
        BMG160_SLEEP_DUR_10                     = 4,
        BMG160_SLEEP_DUR_15                     = 5,
        BMG160_SLEEP_DUR_18                     = 6,
        BMG160_SLEEP_DUR_20                     = 7
    } BMG160_SLEEP_DUR_T;

    /**
     * POWER_MODE values
     */
    typedef enum {
        BMG160_POWER_MODE_NORMAL                = 0,
        BMG160_POWER_MODE_DEEP_SUSPEND          = 1,
        BMG160_POWER_MODE_SUSPEND               = 4
    } BMG160_POWER_MODE_T;

    /**
     * REG_LPM2 bits
     */
    typedef enum {
        _BMG160_LPM2_RESERVED_BITS              = 0x08,

        BMG160_LPM2_AUTOSLEEP_DUR0              = 0x01,
        BMG160_LPM2_AUTOSLEEP_DUR1              = 0x02,
        BMG160_LPM2_AUTOSLEEP_DUR2              = 0x04,
        _BMG160_LPM2_AUTOSLEEP_DUR_MASK         = 7,
        _BMG160_LPM2_AUTOSLEEP_DUR_SHIFT        = 0,

        BMG160_LPM2_EXT_TRIG_SEL0               = 0x10,
        BMG160_LPM2_EXT_TRIG_SEL1               = 0x20,
        _BMG160_LPM2_EXT_TRIG_SEL_MASK          = 3,
        _BMG160_LPM2_EXT_TRIG_SEL_SHIFT         = 4,

        BMG160_LPM2_POWER_SAVE_MODE             = 0x40,
        BMG160_LPM2_FAST_POWERUP                = 0x80
    } BMG160_LPM2_BITS_T;


    /**
     * LPM2_AUTOSLEEP_DUR values
     */
    typedef enum {
        BMG160_AUTOSLEEP_DUR_NONE               = 0,
        BMG160_AUTOSLEEP_DUR_4MS                = 1,
        BMG160_AUTOSLEEP_DUR_5MS                = 2,
        BMG160_AUTOSLEEP_DUR_8MS                = 3,
        BMG160_AUTOSLEEP_DUR_10MS               = 4,
        BMG160_AUTOSLEEP_DUR_15MS               = 5,
        BMG160_AUTOSLEEP_DUR_20MS               = 6,
        BMG160_AUTOSLEEP_DUR_40MS               = 7
    } BMG160_AUTOSLEEP_DUR_T;

    /**
     * LPM2_EXT_TRIG_SEL values
     */
    typedef enum {
        BMG160_EXT_TRIG_SEL_NONE                = 0,
        BMG160_EXT_TRIG_SEL_INT1                = 1,
        BMG160_EXT_TRIG_SEL_INT2                = 2,
        BMG160_EXT_TRIG_SEL_SDO                 = 3 // if SPI3 mode
                                                    // (unsupported)
    } BMG160_EXT_TRIG_SEL_T;

    /**
     * REG_RATE_HBW bits
     */
    typedef enum {
        _BMG160_RATE_HBW_RESERVED_BITS          = 0x0f | 0x10 | 0x20,

        BMG160_RATE_HBW_SHADOW_DIS              = 0x40,
        BMG160_RATE_HBW_DATA_HIGH_BW            = 0x80
    } BMG160_RATE_HBW_BITS_T;

    /**
     * REG_INT_EN_0 bits
     */
    typedef enum {
        _BMG160_INT_EN_0_RESERVED_BITS          = 0x20 | 0x10 | 0x08 | 0x02
                                                  | 0x01,

        BMG160_INT_EN_0_AUTO_OFFSET_EN          = 0x04,

        BMG160_INT_EN_0_FIFO_EN                 = 0x40,
        BMG160_INT_EN_0_DATA_EN                 = 0x80
    } BMG160_INT_EN_0_BITS_T;

    /**
     * REG_INT_EN_1 bits
     */
    typedef enum {
        _BMG160_INT_EN_1_INT1_RESERVED_BITS     = 0xf0,

        BMG160_INT_EN_1_INT1_LVL                = 0x01, // level or edge
        BMG160_INT_EN_1_INT1_OD                 = 0x02, // push-pull
                                                        // or open
                                                        // drain
        BMG160_INT_EN_1_INT2_LVL                = 0x04,
        BMG160_INT_EN_1_INT2_OD                 = 0x08
    } BMG160_INT_EN_1_BITS_T;

    /**
     * REG_INT_MAP_0 bits
     */
    typedef enum {
        _BMG160_INT_MAP_0_RESERVED_BITS         = 0xf0 | 0x04 | 0x01,

        BMG160_INT_MAP_0_INT1_ANY               = 0x02,
        BMG160_INT_MAP_0_INT1_HIGH              = 0x08
    } BMG160_INT_MAP_0_BITS_T;

    /**
     * REG_INT_MAP_1 bits
     */
    typedef enum {
        BMG160_INT_MAP_1_INT1_DATA              = 0x01,
        BMG160_INT_MAP_1_INT1_FAST_OFFSET       = 0x02,
        BMG160_INT_MAP_1_INT1_FIFO              = 0x04,
        BMG160_INT_MAP_1_INT1_AUTO_OFFSET       = 0x08,
        BMG160_INT_MAP_1_INT2_AUTO_OFFSET       = 0x10,
        BMG160_INT_MAP_1_INT2_FIFO              = 0x20,
        BMG160_INT_MAP_1_INT2_FAST_OFFSET       = 0x40,
        BMG160_INT_MAP_1_INT2_DATA              = 0x80
    } BMG160_INT_MAP_1_BITS_T;

    /**
     * REG_INT_1A bits
     */
    typedef enum {
        _BMG160_INT_1A_RESERVED_BITS           = 0xd5,

        BMG160_INT_1A_ANY_UNFILT_DATA          = 0x02,
        BMG160_INT_1A_HIGH_UNFILT_DATA         = 0x08,
        BMG160_INT_1A_SLOW_OFFSET_UNFILT       = 0x20
    } BMG160_INT_1A_BITS_T;

    /**
     * REG_INT_1B bits
     */
    typedef enum {
        BMG160_INT_1B_ANY_TH0                  = 0x01,
        BMG160_INT_1B_ANY_TH1                  = 0x02,
        BMG160_INT_1B_ANY_TH2                  = 0x04,
        BMG160_INT_1B_ANY_TH3                  = 0x08,
        BMG160_INT_1B_ANY_TH4                  = 0x10,
        BMG160_INT_1B_ANY_TH5                  = 0x20,
        BMG160_INT_1B_ANY_TH6                  = 0x40,
        _BMG160_INT_1B_ANY_TH_MASK             = 127,
        _BMG160_INT_1B_ANY_TH_SHIFT            = 0,

        BMG160_INT_1B_FAST_OFFSET_UNFILT       = 0x80
    } BMG160_INT_1B_BITS_T;

    /**
     * REG_INT_1C bits
     */
    typedef enum {
        _BMG160_INT_1C_RESERVED_BITS           = 0x08,

        BMG160_INT_1C_ANY_EN_X                 = 0x01,
        BMG160_INT_1C_ANY_EN_Y                 = 0x02,
        BMG160_INT_1C_ANY_EN_Z                 = 0x04,

        BMG160_INT_1C_ANY_DUR_SAMPLE0          = 0x10,
        BMG160_INT_1C_ANY_DUR_SAMPLE1          = 0x20,
        BMG160_INT_1C_ANY_DUR_SAMPLE_MASK      = 3,
        BMG160_INT_1C_ANY_DUR_SAMPLE_SHIFT     = 4,

        BMG160_INT_1C_AWAKE_DUR0               = 0x40,
        BMG160_INT_1C_AWAKE_DUR1               = 0x80,
        BMG160_INT_1C_AWAKE_DUR_MASK           = 3,
        BMG160_INT_1C_AWAKE_DUR_SHIFT          = 6
    } BMG160_INT_1C_BITS_T;

    /**
     * INT_1C_ANY_DUR_SAMPLE values
     */
    typedef enum {
        BMG160_ANY_DUR_SAMPLE_4                 = 0, // samples
        BMG160_ANY_DUR_SAMPLE_8                 = 1,
        BMG160_ANY_DUR_SAMPLE_12                = 2,
        BMG160_ANY_DUR_SAMPLE_16                = 3
    } BMG160_ANY_DUR_SAMPLE_T;

    /**
     * INT_1C_AWAKE_DUR values
     */
    typedef enum {
        BMG160_AWAKE_DUR_SAMPLE_8               = 0, // samples
        BMG160_AWAKE_DUR_SAMPLE_16              = 1,
        BMG160_AWAKE_DUR_SAMPLE_32              = 2,
        BMG160_AWAKE_DUR_SAMPLE_64              = 3
    } BMG160_AWAKE_DUR_SAMPLE_T;

    /**
     * REG_INT_1E bits
     */
    typedef enum {
        _BMG160_INT_1E_RESERVED_BITS           = 0x7f,

        BMG160_INT_1E_FIFO_WM_EN               = 0x80
    } BMG160_INT_1E_BITS_T;

    /**
     * REG_INT_RST_LATCH bits
     */
    typedef enum {
        _BMG160_INT_RST_LATCH_RESERVED_BITS     = 0x20,

        BMG160_INT_RST_LATCH0                   = 0x01,
        BMG160_INT_RST_LATCH1                   = 0x02,
        BMG160_INT_RST_LATCH2                   = 0x04,
        BMG160_INT_RST_LATCH3                   = 0x08,
        _BMG160_INT_RST_LATCH_MASK              = 15,
        _BMG160_INT_RST_LATCH_SHIFT             = 0,

        BMG160_INT_RST_LATCH_STATUS_BIT         = 0x10,

        BMG160_INT_RST_LATCH_OFFSET_RESET       = 0x40,
        BMG160_INT_RST_LATCH_RESET_INT          = 0x80
    } BMG160_INT_RST_LATCH_BITS_T;

    /**
     * RST_LATCH values
     */
    typedef enum {
        BMG160_RST_LATCH_NON_LATCHED            = 0,
        BMG160_RST_LATCH_TEMPORARY_250MS        = 1,
        BMG160_RST_LATCH_TEMPORARY_500MS        = 2,
        BMG160_RST_LATCH_TEMPORARY_1S           = 3,
        BMG160_RST_LATCH_TEMPORARY_2S           = 4,
        BMG160_RST_LATCH_TEMPORARY_4S           = 5,
        BMG160_RST_LATCH_TEMPORARY_8S           = 6,
        BMG160_RST_LATCH_LATCHED                = 7,

        // 8 == non latched

        BMG160_RST_LATCH_TEMPORARY_250US        = 9,
        BMG160_RST_LATCH_TEMPORARY_500US        = 10,
        BMG160_RST_LATCH_TEMPORARY_1MS          = 11,
        BMG160_RST_LATCH_TEMPORARY_12_5MS       = 12,
        BMG160_RST_LATCH_TEMPORARY_25MS         = 13,
        BMG160_RST_LATCH_TEMPORARY_50MS         = 14

        // 15 == latched
    } BMG160_RST_LATCH_T;

    /**
     * REG_HIGH_TH_* (X, Y, and Z) register bits
     */
    typedef enum {
        BMG160_HIGH_TH_EN                       = 0x01,

        BMG160_HIGH_TH_TH0                      = 0x02,
        BMG160_HIGH_TH_TH1                      = 0x04,
        BMG160_HIGH_TH_TH2                      = 0x08,
        BMG160_HIGH_TH_TH3                      = 0x10,
        BMG160_HIGH_TH_TH4                      = 0x20,
        _BMG160_HIGH_TH_TH_MASK                 = 31,
        _BMG160_HIGH_TH_TH_SHIFT                = 1,

        BMG160_HIGH_TH_HY0                      = 0x40,
        BMG160_HIGH_TH_HY1                      = 0x80,
        _BMG160_HIGH_TH_HY_MASK                 = 3,
        _BMG160_HIGH_TH_HY_SHIFT                = 6
    } BMG160_HIGH_TH_BITS_T;

    /**
     * REG_SOC bits
     */
    typedef enum {
        BMG160_SOC_SLOW_OFFSET_EN_X             = 0x01,
        BMG160_SOC_SLOW_OFFSET_EN_Y             = 0x02,
        BMG160_SOC_SLOW_OFFSET_EN_Z             = 0x04,

        BMG160_SOC_SLOW_OFFSET_DUR0             = 0x08,
        BMG160_SOC_SLOW_OFFSET_DUR1             = 0x10,
        BMG160_SOC_SLOW_OFFSET_DUR2             = 0x20,
        _BMG160_SOC_SLOW_OFFSET_DUR_MASK        = 7,
        _BMG160_SOC_SLOW_OFFSET_DUR_SHIFT       = 3,

        BMG160_SOC_SLOW_OFFSET_TH0              = 0x40,
        BMG160_SOC_SLOW_OFFSET_TH1              = 0x80,
        _BMG160_SOC_SLOW_OFFSET_TH_MASK         = 3,
        _BMG160_SOC_SLOW_OFFSET_TH_SHIFT        = 6
    } BMG160_SOC_BITS_T;

    /**
     * SOC_SLOW_OFFSET_DUR values
     */
    typedef enum {
        BMG160_SLOW_OFFSET_DUR_40MS             = 0, // 40ms
        BMG160_SLOW_OFFSET_DUR_80MS             = 1,
        BMG160_SLOW_OFFSET_DUR_160MS            = 2,
        BMG160_SLOW_OFFSET_DUR_320MS            = 3,
        BMG160_SLOW_OFFSET_DUR_640MS            = 4,
        BMG160_SLOW_OFFSET_DUR_1280MS           = 5
    } BMG160_SLOW_OFFSET_DUR_T;

    /**
     * SOC_SLOW_OFFSET_TH values
     */
    typedef enum {
        BMG160_SLOW_OFFSET_TH_0_1               = 0, // 0.1 degree/s
        BMG160_SLOW_OFFSET_TH_0_2               = 1,
        BMG160_SLOW_OFFSET_TH_0_5               = 2,
        BMG160_SLOW_OFFSET_TH_1                 = 3
    } BMG160_SLOW_OFFSET_TH_T;

    /**
     * REG_A_FOC bits
     */
    typedef enum {
        BMG160_A_FOC_FAST_OFFSET_EN_X           = 0x01,
        BMG160_A_FOC_FAST_OFFSET_EN_Y           = 0x02,
        BMG160_A_FOC_FAST_OFFSET_EN_Z           = 0x04,

        BMG160_A_FOC_FAST_OFFSET_EN             = 0x08,

        BMG160_A_FOC_FAST_OFFSET_WORDLENGTH0    = 0x10,
        BMG160_A_FOC_FAST_OFFSET_WORDLENGTH1    = 0x20,
        _BMG160_A_FOC_FAST_OFFSET_WORDLENGTH_MASK = 3,
        _BMG160_A_FOC_FAST_OFFSET_WORDLENGTH_SHIFT = 4,

        BMG160_A_FOC_AUTO_OFFSET_WORDLENGTH0    = 0x40,
        BMG160_A_FOC_AUTO_OFFSET_WORDLENGTH1    = 0x80,
        _BMG160_A_FOC_AUTO_OFFSET_WORDLENGTH_MASK = 3,
        _BMG160_A_FOC_AUTO_OFFSET_WORDLENGTH_SHIFT = 6
    } BMG160_A_FOC_BITS_T;

    /**
     * FAST_OFFSET_WORDLENGTH values
     */
    typedef enum {
        BMG160_FAST_OFFSET_WORDLENGTH_32        = 0, // samples
        BMG160_FAST_OFFSET_WORDLENGTH_64        = 1,
        BMG160_FAST_OFFSET_WORDLENGTH_128       = 2,
        BMG160_FAST_OFFSET_WORDLENGTH_256       = 3
    } BMG160_FAST_OFFSET_WORDLENGTH_T;

    /**
     * AUTO_OFFSET_WORDLENGTH values
     */
    typedef enum {
        BMG160_AUTO_OFFSET_WORDLENGTH_32        = 0, // samples
        BMG160_AUTO_OFFSET_WORDLENGTH_64        = 1,
        BMG160_AUTO_OFFSET_WORDLENGTH_128       = 2,
        BMG160_AUTO_OFFSET_WORDLENGTH_256       = 3
    } BMG160_AUTO_OFFSET_WORDLENGTH_T;

    /**
     * REG_TRIM_NVM_CTRL bits
     */
    typedef enum {
        BMG160_TRIM_NVM_CTRL_NVM_PROG_MODE     = 0x01,
        BMG160_TRIM_NVM_CTRL_NVM_PROG_TRIG     = 0x02,
        BMG160_TRIM_NVM_CTRL_NVM_PROG_RDY      = 0x04,
        BMG160_TRIM_NVM_CTRL_NVM_PROG_LOAD     = 0x08,

        BMG160_TRIM_NVM_CTRL_NVM_REMAIN0       = 0x10,
        BMG160_TRIM_NVM_CTRL_NVM_REMAIN1       = 0x20,
        BMG160_TRIM_NVM_CTRL_NVM_REMAIN2       = 0x40,
        BMG160_TRIM_NVM_CTRL_NVM_REMAIN3       = 0x80,
        _BMG160_TRIM_NVM_CTRL_NVM_REMAIN_MASK  = 15,
        _BMG160_TRIM_NVM_CTRL_NVM_REMAIN_SHIFT = 4
    } BMG160_TRIM_NVM_CTRL_BITS_T;

    /**
     * REG_SPI3_WDT bits
     */
    typedef enum {
        _BMG160_SPI3_WDT_RESERVED_BITS         = 0xf0 | 0x08,

        BMG160_SPI3_WDT_SPI3                   = 0x01, // 3-wire SPI -
                                                       // NOT
                                                       // SUPPORTED

        BMG160_SPI3_WDT_I2C_WDT_SEL            = 0x02,
        BMG160_SPI3_WDT_I2C_WDT_EN             = 0x04

        // 0x08-0x80 reserved
    } BMG160_SPI3_WDT_BITS_T;

    /**
     * REG_OFC1 bits, the missing x, y, and z llsb bits are in GP0
     */
    typedef enum {
        BMG160_OFC1_OFFSET_Z0                  = 0x01, // Z lsb (3:1)
        BMG160_OFC1_OFFSET_Z1                  = 0x02,
        BMG160_OFC1_OFFSET_Z2                  = 0x04,
        _BMG160_OFC1_OFFSET_Z_MASK             = 7,
        _BMG160_OFC1_OFFSET_Z_SHIFT            = 0,

        BMG160_OFC1_OFFSET_Y0                  = 0x08, // Y lsb (3:1)
        BMG160_OFC1_OFFSET_Y1                  = 0x10,
        BMG160_OFC1_OFFSET_Y2                  = 0x20,
        _BMG160_OFC1_OFFSET_Y_MASK             = 7,
        _BMG160_OFC1_OFFSET_Y_SHIFT            = 3,

        BMG160_OFC1_OFFSET_X0                  = 0x08, // bits 3:2 of
                                                       // X lsb. geez
        BMG160_OFC1_OFFSET_X1                  = 0x10,
        _BMG160_OFC1_OFFSET_X_MASK             = 3,
        _BMG160_OFC1_OFFSET_X_SHIFT            = 6
    } BMG160_OFC1_OFFSET_BITS_T;

    /**
     * REG_GP0 bits
     */
    typedef enum {
        BMG160_GP0_OFFSET_Z                    = 0x01, // Z llsb (bit 0)
        BMG160_GP0_OFFSET_Y                    = 0x02, // Y llsb (bit 0)

        BMG160_GP0_OFFSET_X0                   = 0x04, // X llsbs (bits 1:0)
        BMG160_GP0_OFFSET_X1                   = 0x08,
        _BMG160_GP0_OFFSET_X_MASK              = 3,
        _BMG160_GP0_OFFSET_X_SHIFT             = 2,

        BMG160_GP0_GP00                        = 0x10,
        BMG160_GP0_GP01                        = 0x20,
        BMG160_GP0_GP02                        = 0x40,
        BMG160_GP0_GP03                        = 0x80,
        _BMG160_GP0_GP0_MASK                   = 15,
        _BMG160_GP0_GP0_SHIFT                  = 4
    } BMG160_GP0_BITS_T;

    /**
     * REG_BIST bits
     */
    typedef enum {
        _BMG160_BIST_RESERVED_BITS             = 0x80 | 0x40 | 0x20 | 0x08,

        BMG160_BIST_TRIG_BIST                  = 0x01,
        BMG160_BIST_BIST_RDY                   = 0x02,
        BMG160_BIST_BIST_FAIL                  = 0x04,

        BMG160_BIST_RATE_OK                    = 0x10
    } BMG160_BIST_BITS_T;

    /**
     * REG_FIFO_CONFIG_0 bits
     */
    typedef enum {
        BMG160_FIFO_CONFIG_0_WATER_MARK0       = 0x01,
        BMG160_FIFO_CONFIG_0_WATER_MARK1       = 0x02,
        BMG160_FIFO_CONFIG_0_WATER_MARK2       = 0x04,
        BMG160_FIFO_CONFIG_0_WATER_MARK3       = 0x08,
        BMG160_FIFO_CONFIG_0_WATER_MARK4       = 0x10,
        BMG160_FIFO_CONFIG_0_WATER_MARK5       = 0x20,
        BMG160_FIFO_CONFIG_0_WATER_MARK6       = 0x40,
        _BMG160_FIFO_CONFIG_0_WATER_MARK_MASK  = 127,
        _BMG160_FIFO_CONFIG_0_WATER_MARK_SHIFT = 0,

        BMG160_FIFO_CONFIG_0_TAG               = 0x80
    } BMG160_FIFO_CONFIG_0_BITS_T;

    /**
     * REG_FIFO_CONFIG_1 bits
     */
    typedef enum {
        _BMG160_FIFO_CONFIG_1_RESERVED_BITS     = 0x20 | 0x10 |0x08 | 0x04,

        BMG160_FIFO_CONFIG_1_FIFO_DATA_SEL0     = 0x01,
        BMG160_FIFO_CONFIG_1_FIFO_DATA_SEL1     = 0x02,
        _BMG160_FIFO_CONFIG_1_FIFO_DATA_SEL     = 3,
        _BMG160_FIFO_CONFIG_1_FIFO_DATA_SHIFT   = 0,

        BMG160_FIFO_CONFIG_1_FIFO_MODE0         = 0x40,
        BMG160_FIFO_CONFIG_1_FIFO_MODE1         = 0x80,
        _BMG160_FIFO_CONFIG_1_FIFO_MODE_MASK    = 3,
        _BMG160_FIFO_CONFIG_1_FIFO_MODE_SHIFT   = 6
    } BMG160_FIFO_CONFIG_1_BITS_T;

    /**
     * FIFO_DATA_SEL values
     */
    typedef enum {
        BMG160_FIFO_DATA_SEL_XYZ               = 0,
        BMG160_FIFO_DATA_SEL_X                 = 1,
        BMG160_FIFO_DATA_SEL_Y                 = 2,
        BMG160_FIFO_DATA_SEL_Z                 = 3
    } BMG160_FIFO_DATA_SEL_T;

    /**
     * FIFO_MODE values
     */
    typedef enum {
        BMG160_FIFO_MODE_BYPASS                = 0,
        BMG160_FIFO_MODE_FIFO                  = 1,
        BMG160_FIFO_MODE_STREAM                = 2
    } BMG160_FIFO_MODE_T;

    // interrupt selection for installISR() and uninstallISR()
    typedef enum {
        BMG160_INTERRUPT_INT1,
        BMG160_INTERRUPT_INT2
    } BMG160_INTERRUPT_PINS_T;

#ifdef __cplusplus
}
#endif
