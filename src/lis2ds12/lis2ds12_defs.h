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

#define LIS2DS12_DEFAULT_I2C_BUS 0
#define LIS2DS12_DEFAULT_SPI_BUS 0
#define LIS2DS12_DEFAULT_I2C_ADDR 0x1e


#define LIS2DS12_CHIPID 0x43

    // NOTE: Reserved registers must not be written into or permanent
    // damage can result.  Reading from them may return indeterminate
    // values.  Registers containing reserved bitfields must be
    // written as 0.

    /**
     * LIS2DS12 registers
     */
    typedef enum {
        // 0x00-0x05 reserved

        LIS2DS12_REG_SENSORHUB1_REG               = 0x06,
        LIS2DS12_REG_SENSORHUB2_REG               = 0x07,
        LIS2DS12_REG_SENSORHUB3_REG               = 0x08,
        LIS2DS12_REG_SENSORHUB4_REG               = 0x09,
        LIS2DS12_REG_SENSORHUB5_REG               = 0x0a,
        LIS2DS12_REG_SENSORHUB6_REG               = 0x0b,

        LIS2DS12_REG_MODULE_8BIT                  = 0x0c,

        // 0x0d-0x0e reserved

        LIS2DS12_REG_WHO_AM_I                     = 0x0f,

        // 0x10-0x1f reserved

        LIS2DS12_REG_CTRL1                        = 0x20,
        LIS2DS12_REG_CTRL2                        = 0x21,
        LIS2DS12_REG_CTRL3                        = 0x22,
        LIS2DS12_REG_CTRL4                        = 0x23,
        LIS2DS12_REG_CTRL5                        = 0x24,

        LIS2DS12_REG_FIFO_CTRL                    = 0x25,

        LIS2DS12_REG_OUT_T                        = 0x26,

        LIS2DS12_REG_STATUS                       = 0x27,

        LIS2DS12_REG_OUT_X_L                      = 0x28,
        LIS2DS12_REG_OUT_X_H                      = 0x29,
        LIS2DS12_REG_OUT_Y_L                      = 0x2a,
        LIS2DS12_REG_OUT_Y_H                      = 0x2b,
        LIS2DS12_REG_OUT_Z_L                      = 0x2c,
        LIS2DS12_REG_OUT_Z_H                      = 0x2d,

        LIS2DS12_REG_FIFO_THS                     = 0x2e,
        LIS2DS12_REG_FIFO_SRC                     = 0x2f,
        LIS2DS12_REG_FIFO_SAMPLES                 = 0x30,

        LIS2DS12_REG_TAP_6D_THS                   = 0x31,
        LIS2DS12_REG_INT_DUR                      = 0x32,

        LIS2DS12_REG_WAKE_UP_THS                  = 0x33,
        LIS2DS12_REG_WAKE_UP_DUR                  = 0x34,

        LIS2DS12_REG_FREE_FALL                    = 0x35,

        LIS2DS12_REG_STATUS_DUP                   = 0x36,

        LIS2DS12_REG_WAKE_UP_SRC                  = 0x37,

        LIS2DS12_REG_TAP_SRC                      = 0x38,
        LIS2DS12_REG_6D_SRC                       = 0x39,

        LIS2DS12_REG_STEP_COUNTER_MINTHS          = 0x3a,
        LIS2DS12_REG_STEP_COUNTER_L               = 0x3b,
        LIS2DS12_REG_STEP_COUNTER_H               = 0x3c,

        LIS2DS12_REG_FUNC_CK_GATE                 = 0x3d,
        LIS2DS12_REG_FUNC_SRC                     = 0x3e,
        LIS2DS12_REG_FUNC_CTRL                    = 0x3f,
    } LIS2DS12_REGS_T;

    /**
     * REG_CTRL1 bits
     */
    typedef enum {
        LIS2DS12_CTRL1_BDU                        = 0x01,
        LIS2DS12_CTRL1_HF_ODR                     = 0x02,

        LIS2DS12_CTRL1_FS0                        = 0x04,
        LIS2DS12_CTRL1_FS1                        = 0x08,
        _LIS2DS12_CTRL1_FS_MASK                   = 3,
        _LIS2DS12_CTRL1_FS_SHIFT                  = 2,

        LIS2DS12_CTRL1_ODR0                       = 0x10,
        LIS2DS12_CTRL1_ODR1                       = 0x20,
        LIS2DS12_CTRL1_ODR2                       = 0x40,
        LIS2DS12_CTRL1_ODR3                       = 0x80,
        _LIS2DS12_CTRL1_ODR_MASK                  = 15,
        _LIS2DS12_CTRL1_ODR_SHIFT                 = 4,
    } LIS2DS12_CTRL1_BITS_T;

    /**
     * CTRL1_FS (full scale) values
     */
    typedef enum {
        LIS2DS12_FS_2G                            = 0,
        // odd ordering in the DS...
        LIS2DS12_FS_16G                           = 1,
        LIS2DS12_FS_4G                            = 2,
        LIS2DS12_FS_8G                            = 3,
    } LIS2DS12_FS_T;

    /**
     * CTRL1_ODR (Output Data Rate) values
     */
    typedef enum {
        LIS2DS12_ODR_POWER_DOWN                   = 0,

        // high resolution modes (14 bit resolution)

        LIS2DS12_ODR_12_5HZ                       = 1,
        LIS2DS12_ODR_25HZ                         = 2,
        LIS2DS12_ODR_50HZ                         = 3,
        LIS2DS12_ODR_100HZ                        = 4,

        // The following items (5, 6, 7) have a dual meaning depending
        // on whether the HF_ODR bit is set, but they use the same
        // overlapping ODR values for the ODR bitfield.  Since the
        // bitfield is only 4 bits wide, we add a "virtual" 5th bit to
        // indicate the HF versions.  This is then screened out in the
        // code and will set the HF bit according to what is selected
        // here.

        // CTRL1_HF_ODR == 0 (14 bit resolution)
        LIS2DS12_ODR_200HZ                        = 5,
        LIS2DS12_ODR_400HZ                        = 6,
        LIS2DS12_ODR_800HZ                        = 7,

        // CTRL1_HF_ODR == 1 (12bit resolution). Add 'virtual' bit 5
        // value (16) for these HF modes, which we will detect and
        // screen out in the driver.  This simplifies the ODR API.
        LIS2DS12_ODR_1600HZ                       = (16 + 5),
        LIS2DS12_ODR_3200HZ                       = (16 + 6),
        LIS2DS12_ODR_6400HZ                       = (16 + 7),

        // low power modes (10 bit resolution)
        LIS2DS12_ODR_LP_1HZ                       = 8,
        LIS2DS12_ODR_LP_12_5HZ                    = 9,
        LIS2DS12_ODR_LP_25HZ                      = 10,
        LIS2DS12_ODR_LP_50HZ                      = 11,
        LIS2DS12_ODR_LP_100HZ                     = 12,
        LIS2DS12_ODR_LP_200HZ                     = 13,
        LIS2DS12_ODR_LP_400HZ                     = 14,
        LIS2DS12_ODR_LP_800HZ                     = 15,
    } LIS2DS12_ODR_T;

    /**
     * REG_CTRL2 bits
     */
    typedef enum {
        LIS2DS12_CTRL2_SIM                        = 0x01, // SPI 3 wire enable
        LIS2DS12_CTRL2_I2C_DISABLE                = 0x02,
        LIS2DS12_CTRL2_IF_ADD_INC                 = 0x04, // auto-increment
        LIS2DS12_CTRL2_FDS_SLOPE                  = 0x08,
        LIS2DS12_CTRL2_FUNC_CFG_ENABLE            = 0x10,

        // 0x20 reserved

        LIS2DS12_CTRL2_SOFT_RESET                 = 0x40,
        LIS2DS12_CTRL2_BOOT                       = 0x80,
    } LIS2DS12_CTRL2_BITS_T;

    /**
     * REG_CTRL3 bits
     */
    typedef enum {
        LIS2DS12_CTRL3_PP_OD                      = 0x01, //push-pull/open-drain
        LIS2DS12_CTRL3_H_LACTIVE                  = 0x02,
        LIS2DS12_CTRL3_LIR                        = 0x04,
        LIS2DS12_CTRL3_TAP_Z_EN                   = 0x08,
        LIS2DS12_CTRL3_TAP_Y_EN                   = 0x10,
        LIS2DS12_CTRL3_TAP_X_EN                   = 0x20,

        LIS2DS12_CTRL3_ST0                        = 0x40,
        LIS2DS12_CTRL3_ST1                        = 0x80,
        _LIS2DS12_CTRL3_ST_MASK                   = 3,
        _LIS2DS12_CTRL3_ST_SHIFT                  = 6,
    } LIS2DS12_CTRL3_BITS_T;

    /**
     * CTRL3_ST (Self Test) values
     */
    typedef enum {
        LIS2DS12_ST_NORMAL                        = 0,
        LIS2DS12_ST_POS_SIGN                      = 1,
        LIS2DS12_ST_NEG_SIGN                      = 2
    } LIS2DS12_ST_T;

    /**
     * REG_CTRL4 bits
     */
    typedef enum {
        LIS2DS12_CTRL4_INT1_DRDY                  = 0x01,
        LIS2DS12_CTRL4_INT1_FTH                   = 0x02,
        LIS2DS12_CTRL4_INT1_6D                    = 0x04,
        LIS2DS12_CTRL4_INT1_TAP                   = 0x08,
        LIS2DS12_CTRL4_INT1_FF                    = 0x10,
        LIS2DS12_CTRL4_INT1_WU                    = 0x20,
        LIS2DS12_CTRL4_INT1_S_TAP                 = 0x40,
        LIS2DS12_CTRL4_INT1_MASTER_DRDY           = 0x80,
    } LIS2DS12_CTRL4_BITS_T;

    /**
     * REG_CTRL5 bits
     */
    typedef enum {
        LIS2DS12_CTRL5_INT2_DRDY                  = 0x01,
        LIS2DS12_CTRL5_INT2_FTH                   = 0x02,
        LIS2DS12_CTRL5_INT2_STEP_DET              = 0x04,
        LIS2DS12_CTRL5_INT2_SIG_MOT               = 0x08,
        LIS2DS12_CTRL5_INT2_TILT                  = 0x10,
        LIS2DS12_CTRL5_INT2_ON_INT1               = 0x20,
        LIS2DS12_CTRL5_INT2_BOOT                  = 0x40,
        LIS2DS12_CTRL5_INT2_DRDY_PULSED           = 0x80,
    } LIS2DS12_CTRL5_BITS_T;

    /**
     * REG_FIFO_CTRL bits
     */
    typedef enum {
        LIS2DS12_FIFO_CTRL_IF_CS_PU_DIS           = 0x01,

        // 0x02-0x04 reserved

        LIS2DS12_FIFO_CTRL_MODULE_TO_FIFO         = 0x08,
        LIS2DS12_FIFO_CTRL_INT2_STEP_COUNT_OV     = 0x10,

        LIS2DS12_FIFO_CTRL_FMODE0                 = 0x20,
        LIS2DS12_FIFO_CTRL_FMODE1                 = 0x40,
        LIS2DS12_FIFO_CTRL_FMODE2                 = 0x80,
        _LIS2DS12_FIFO_CTRL_FMODE_MASK            = 7,
        _LIS2DS12_FIFO_CTRL_FMODE_SHIFT           = 5,
    } LIS2DS12_FIFO_CTRL_BITS_T;

    /**
     * FIFO_CTRL_FMODE (FIFO mode) values
     */
    typedef enum {
        LIS2DS12_FMODE_BYPASS                     = 0,
        LIS2DS12_FMODE_FIFO                       = 1,
        // 2 reserved
        LIS2DS12_FMODE_CONT_TO_FIFO               = 3,
        LIS2DS12_FMODE_BYPASS_TO_CONT             = 4,
        // 5 reserved
        LIS2DS12_FMODE_CONT                       = 6,
        // 7 reserved
    } LIS2DS12_FMODE_T;

    /**
     * REG_STATUS bits
     */
    typedef enum {
        LIS2DS12_STATUS_DRDY                      = 0x01,
        LIS2DS12_STATUS_FF_IA                     = 0x02,
        LIS2DS12_STATUS_6D_IA                     = 0x04,
        LIS2DS12_STATUS_SINGLE_TAP                = 0x08,
        LIS2DS12_STATUS_DOUBLE_TAP                = 0x10,
        LIS2DS12_STATUS_SLEEP_STATE               = 0x20,
        LIS2DS12_STATUS_WU_IA                     = 0x40,
        LIS2DS12_STATUS_FIFO_THS                  = 0x80,
    } LIS2DS12_STATUS_BITS_T;

    /**
     * REG_FIFO_SRC bits
     */
    typedef enum {
        // 0x01-0x10 reserved
        LIS2DS12_FIFO_SRC_DIFF8                   = 0x20,
        LIS2DS12_FIFO_SRC_FIFO_OVR                = 0x40,
        LIS2DS12_FIFO_SRC_FTH                     = 0x80,
    } LIS2DS12_FIFO_SRC_BITS_T;

    /**
     * REG_TAP_6D_THS bits
     */
    typedef enum {
        LIS2DS12_TAP_6D_THS_TAP_THS0              = 0x01,
        LIS2DS12_TAP_6D_THS_TAP_THS1              = 0x02,
        LIS2DS12_TAP_6D_THS_TAP_THS2              = 0x04,
        LIS2DS12_TAP_6D_THS_TAP_THS3              = 0x08,
        LIS2DS12_TAP_6D_THS_TAP_THS4              = 0x10,
        _LIS2DS12_TAP_6D_THS_TAP_THS_MASK         = 31,
        _LIS2DS12_TAP_6D_THS_TAP_THS_SHIFT        = 0,

        LIS2DS12_TAP_6D_THS_6D_THS0               = 0x20,
        LIS2DS12_TAP_6D_THS_6D_THS1               = 0x40,
        _LIS2DS12_TAP_6D_THS_6D_THS_MASK          = 3,
        _LIS2DS12_TAP_6D_THS_6D_THS_SHIFT         = 5,

        LIS2DS12_TAP_6D_THS_6D_4D_EN              = 0x80,

    } LIS2DS12_TAP_6D_THS_BITS_T;

    /**
     * TAP_6D_THS_6D_THS values
     */
    typedef enum {
        LIS2DS12_6D_THS_6                         = 0, // 80 degrees
        LIS2DS12_6D_THS_11                        = 1, // 70 degrees
        LIS2DS12_6D_THS_16                        = 2, // 60 degrees
        LIS2DS12_6D_THS_21                        = 3, // 50 degrees
    } LIS2DS12_6D_THS_T;

    /**
     * REG_INT_DUR bits
     */
    typedef enum {
        LIS2DS12_INT_DUR_SHOCK0                   = 0x01,
        LIS2DS12_INT_DUR_SHOCK1                   = 0x02,
        _LIS2DS12_INT_DUR_SHOCK_MASK              = 3,
        _LIS2DS12_INT_DUR_SHOCK_SHIFT             = 0,

        LIS2DS12_INT_DUR_QUIET0                   = 0x04,
        LIS2DS12_INT_DUR_QUIET1                   = 0x08,
        _LIS2DS12_INT_DUR_QUIET_MASK              = 3,
        _LIS2DS12_INT_DUR_QUIET_SHIFT             = 2,

        LIS2DS12_INT_DUR_LAT0                     = 0x10,
        LIS2DS12_INT_DUR_LAT1                     = 0x20,
        LIS2DS12_INT_DUR_LAT2                     = 0x40,
        LIS2DS12_INT_DUR_LAT3                     = 0x80,
        _LIS2DS12_INT_DUR_LAT_MASK                = 15,
        _LIS2DS12_INT_DUR_LAT_SHIFT               = 4,

    } LIS2DS12_INT_DUR_BITS_T;

    /**
     * REG_WAKE_UP_THS bits
     */
    typedef enum {
        LIS2DS12_WAKE_UP_THS_WU_THS0              = 0x01,
        LIS2DS12_WAKE_UP_THS_WU_THS1              = 0x02,
        LIS2DS12_WAKE_UP_THS_WU_THS2              = 0x04,
        LIS2DS12_WAKE_UP_THS_WU_THS3              = 0x08,
        LIS2DS12_WAKE_UP_THS_WU_THS4              = 0x10,
        LIS2DS12_WAKE_UP_THS_WU_THS5              = 0x20,
        _LIS2DS12_WAKE_UP_THS_WU_THS_MASK         = 63,
        _LIS2DS12_WAKE_UP_THS_WU_THS_SHIFT        = 0,

        LIS2DS12_WAKE_UP_THS_SLEEP_ON             = 0x40,
        LIS2DS12_WAKE_UP_THS_SINGLE_DOUBLE_TAP    = 0x80,
    } LIS2DS12_WAKE_UP_THS_BITS_T;

    /**
     * REG_WAKE_UP_DUR bits
     */
    typedef enum {
        LIS2DS12_WAKE_UP_DUR_SLEEP_DUR0           = 0x01,
        LIS2DS12_WAKE_UP_DUR_SLEEP_DUR1           = 0x02,
        LIS2DS12_WAKE_UP_DUR_SLEEP_DUR2           = 0x04,
        LIS2DS12_WAKE_UP_DUR_SLEEP_DUR3           = 0x08,
        _LIS2DS12_WAKE_UP_DUR_SLEEP_DUR_MASK      = 15,
        _LIS2DS12_WAKE_UP_DUR_SLEEP_DUR_SHIFT     = 0,

        LIS2DS12_WAKE_UP_DUR_INT1_FSS7            = 0x10,

        LIS2DS12_WAKE_UP_DUR_WU_DUR0              = 0x20,
        LIS2DS12_WAKE_UP_DUR_WU_DUR1              = 0x40,
        _LIS2DS12_WAKE_UP_DUR_WU_DUR_MASK         = 3,
        _LIS2DS12_WAKE_UP_DUR_WU_DUR_SHIFT        = 5,

        LIS2DS12_WAKE_UP_DUR_FF_DUR5              = 0x80,
    } LIS2DS12_WAKE_UP_DUR_BITS_T;

    /**
     * REG_FREE_FALL bits
     */
    typedef enum {
        LIS2DS12_FREE_FALL_FF_THS0                = 0x01,
        LIS2DS12_FREE_FALL_FF_THS1                = 0x02,
        LIS2DS12_FREE_FALL_FF_THS2                = 0x04,
        _LIS2DS12_FREE_FALL_FF_THS_MASK           = 7,
        _LIS2DS12_FREE_FALL_FF_THS_SHIFT          = 0,

        LIS2DS12_FREE_FALL_FF_DUR0                = 0x08,
        LIS2DS12_FREE_FALL_FF_DUR1                = 0x10,
        LIS2DS12_FREE_FALL_FF_DUR2                = 0x20,
        LIS2DS12_FREE_FALL_FF_DUR3                = 0x40,
        LIS2DS12_FREE_FALL_FF_DUR4                = 0x80,
        _LIS2DS12_FREE_FALL_FF_MASK               = 31,
        _LIS2DS12_FREE_FALL_FF_SHIFT              = 3,
    } LIS2DS12_FREE_FALL_BITS_T;

    /**
     * REG_STATUS_DUP bits
     */
    typedef enum {
        LIS2DS12_STATUS_DUP_DRDY                  = 0x01,
        LIS2DS12_STATUS_DUP_FF_IA                 = 0x02,
        LIS2DS12_STATUS_DUP_6D_IA                 = 0x04,
        LIS2DS12_STATUS_DUP_SINGLE_TAP            = 0x08,
        LIS2DS12_STATUS_DUP_DOUBLE_TAP            = 0x10,
        LIS2DS12_STATUS_DUP_SLEEP_STATE           = 0x20,
        LIS2DS12_STATUS_DUP_WU_IA                 = 0x40,
        LIS2DS12_STATUS_DUP_OVR                   = 0x80,
    } LIS2DS12_STATUS_DUP_BITS_T;

    /**
     * REG_WAKE_UP_SRC bits
     */
    typedef enum {
        LIS2DS12_WAKE_UP_SRC_Z_WU                 = 0x01,
        LIS2DS12_WAKE_UP_SRC_Y_WU                 = 0x02,
        LIS2DS12_WAKE_UP_SRC_X_WU                 = 0x04,

        LIS2DS12_WAKE_UP_SRC_WU_IA                = 0x08,
        LIS2DS12_WAKE_UP_SRC_SLEEP_STATE_IA       = 0x10,
        LIS2DS12_WAKE_UP_SRC_FF_IA                = 0x20,

        // 0x40-0x80 reserved
    } LIS2DS12_WAKE_UP_SRC_BITS_T;

    /**
     * REG_TAP_SRC bits
     */
    typedef enum {
        LIS2DS12_TAP_SRC_Z_TAP                    = 0x01,
        LIS2DS12_TAP_SRC_Y_TAP                    = 0x02,
        LIS2DS12_TAP_SRC_X_TAP                    = 0x04,
        LIS2DS12_TAP_SRC_TAP_SIGN                 = 0x08,
        LIS2DS12_TAP_SRC_DOUBLE_TAP               = 0x10,
        LIS2DS12_TAP_SRC_SINGLE_TAP               = 0x20,
        LIS2DS12_TAP_SRC_TAP_IA                   = 0x40,

        // 0x80 reserved
    } LIS2DS12_TAP_SRC_BITS_T;

    /**
     * REG_6D_SRC bits
     */
    typedef enum {
        LIS2DS12_6D_SRC_XL                        = 0x01,
        LIS2DS12_6D_SRC_XH                        = 0x02,
        LIS2DS12_6D_SRC_YL                        = 0x04,
        LIS2DS12_6D_SRC_YH                        = 0x08,
        LIS2DS12_6D_SRC_ZL                        = 0x10,
        LIS2DS12_6D_SRC_ZH                        = 0x20,

        LIS2DS12_6D_IA                            = 0x40,

        // 0x80 reserved
    } LIS2DS12_6D_SRC_BITS_T;

    /**
     * REG_STEP_COUNTER_MINTHS bits
     */
    typedef enum {
        LIS2DS12_STEP_COUNTER_MINTHS_SC_MTHS0     = 0x01,
        LIS2DS12_STEP_COUNTER_MINTHS_SC_MTHS1     = 0x02,
        LIS2DS12_STEP_COUNTER_MINTHS_SC_MTHS2     = 0x04,
        LIS2DS12_STEP_COUNTER_MINTHS_SC_MTHS3     = 0x08,
        LIS2DS12_STEP_COUNTER_MINTHS_SC_MTHS4     = 0x10,
        LIS2DS12_STEP_COUNTER_MINTHS_SC_MTHS5     = 0x20,
        _LIS2DS12_STEP_COUNTER_MINTHS_SC_MTHS5_MASK = 63,
        _LIS2DS12_STEP_COUNTER_MINTHS_SC_MTHS5_SHIFT = 0,

        LIS2DS12_STEP_COUNTER_MINTHS_PEDO4G       = 0x40,
        LIS2DS12_STEP_COUNTER_MINTHS_RST_NSTEP    = 0x80,
    } LIS2DS12_STEP_COUNTER_MINTHS_BITS_T;

    /**
     * REG_FUNC_CK_GATE bits
     */
    typedef enum {
        LIS2DS12_FUNC_CK_GATE_CK_GATE_FUNC        = 0x01,
        LIS2DS12_FUNC_CK_GATE_STEP_DETECT         = 0x02,
        LIS2DS12_FUNC_CK_GATE_RST_PEDO            = 0x04,
        LIS2DS12_FUNC_CK_GATE_RST_SIGN_MOT        = 0x08,
        LIS2DS12_FUNC_CK_GATE_SIG_MOT_DETECT      = 0x10,

        LIS2DS12_FUNC_CK_GATE_FS_SRC0             = 0x20,
        LIS2DS12_FUNC_CK_GATE_FS_SRC1             = 0x40,
        _LIS2DS12_FUNC_CK_GATE_FS_SRC_MASK        = 3,
        _LIS2DS12_FUNC_CK_GATE_FS_SRC_SHIFT       = 5,

        LIS2DS12_FUNC_CK_GATE_TILT_INT            = 0x80,
    } LIS2DS12_FUNC_CK_GATE_BITS_T;

    /**
     * FUNC_CK_GATE_FS_SRC values
     */
    typedef enum {
        LIS2DS12_FS_SRC_NO_SCALE                  = 0,
        LIS2DS12_FS_SRC_2G                        = 1,
        LIS2DS12_FS_SRC_4G                        = 2,
    } LIS2DS12_FS_SRC_T;

    /**
     * REG_FUNC_SRC bits
     */
    typedef enum {
        LIS2DS12_FUNC_SRC_SENSORHUB_END_OP        = 0x01,
        LIS2DS12_FUNC_SRC_MODULE_READY            = 0x02,
        LIS2DS12_FUNC_SRC_RST_TILT                = 0x04,

        // 0x08-0x80 reserved
    } LIS2DS12_FUNC_SRC_BITS_T;

    /**
     * REG_FUNC_CTRL bits
     */
    typedef enum {
        LIS2DS12_FUNC_CTRL_STEP_CNT_ON            = 0x01,
        LIS2DS12_FUNC_CTRL_SIGN_MOT_ON            = 0x02,
        LIS2DS12_FUNC_CTRL_MASTER_ON              = 0x04,
        LIS2DS12_FUNC_CTRL_TUD_EN                 = 0x08,
        LIS2DS12_FUNC_CTRL_TILT_ON                = 0x10,
        LIS2DS12_FUNC_CTRL_MODULE_ON              = 0x20,

        // 0x40-0x80 reserved
    } LIS2DS12_FUNC_CTRL_BITS_T;

    // interrupt selection for installISR() and uninstallISR()
    typedef enum {
        LIS2DS12_INTERRUPT_INT1,
        LIS2DS12_INTERRUPT_INT2
    } LIS2DS12_INTERRUPT_PINS_T;

#ifdef __cplusplus
}
#endif
