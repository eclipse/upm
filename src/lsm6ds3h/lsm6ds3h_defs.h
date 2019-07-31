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

#define LSM6DS3H_DEFAULT_I2C_BUS 0
#define LSM6DS3H_DEFAULT_SPI_BUS 0
#define LSM6DS3H_DEFAULT_I2C_ADDR 0x6a

#define LSM6DS3H_CHIPID 0x69

#ifdef __cplusplus
extern "C" {
#endif

    // NOTE: Reserved registers must not be written into or permanent
    // damage can result.  Reading from them may return indeterminate
    // values.  Registers containing reserved bitfields must be
    // written as 0.

    // This register map is not complete -- all registers are
    // enumerated, however not all register bitmaps are enumerated
    // here.  Feel free to add any you need that are missing.

    /**
     * LSM6DS3H registers
     */
    typedef enum {
        // 0x00 reserved

        LSM6DS3H_REG_FUNC_CFG_ACCESS              = 0x01,

        // 0x02-0x03 reserved

        LSM6DS3H_REG_SENSOR_SYNC_TIME_FRAME       = 0x04,

        // 0x05 reserved

        LSM6DS3H_REG_FIFO_CTRL1                   = 0x06,
        LSM6DS3H_REG_FIFO_CTRL2                   = 0x07,
        LSM6DS3H_REG_FIFO_CTRL3                   = 0x08,
        LSM6DS3H_REG_FIFO_CTRL4                   = 0x09,
        LSM6DS3H_REG_FIFO_CTRL5                   = 0x0a,

        LSM6DS3H_REG_ORIENT_CFG_G                 = 0x0b,

        // 0x0c reserved

        LSM6DS3H_REG_INT1_CTRL                    = 0x0d,
        LSM6DS3H_REG_INT2_CTRL                    = 0x0e,

        LSM6DS3H_REG_WHO_AM_I                     = 0x0f,

        LSM6DS3H_REG_CTRL1_XL                     = 0x10,
        LSM6DS3H_REG_CTRL2_G                      = 0x11,
        LSM6DS3H_REG_CTRL3_C                      = 0x12,
        LSM6DS3H_REG_CTRL4_C                      = 0x13,
        LSM6DS3H_REG_CTRL5_C                      = 0x14,
        LSM6DS3H_REG_CTRL6_C                      = 0x15,
        LSM6DS3H_REG_CTRL7_G                      = 0x16,
        LSM6DS3H_REG_CTRL8_XL                     = 0x17,
        LSM6DS3H_REG_CTRL9_XL                     = 0x18,
        LSM6DS3H_REG_CTRL10_C                     = 0x19,

        LSM6DS3H_REG_MASTER_CFG                   = 0x1a,
        LSM6DS3H_REG_WAKE_UP_SRC                  = 0x1b,
        LSM6DS3H_REG_TAP_SRC                      = 0x1c,
        LSM6DS3H_REG_TAP_D6D                      = 0x1d,

        // also STATUS_SPIAux
        LSM6DS3H_REG_STATUS                       = 0x1e,

        // 0x1f reserved

        LSM6DS3H_REG_OUT_TEMP_L                   = 0x20,
        LSM6DS3H_REG_OUT_TEMP_H                   = 0x21,

        LSM6DS3H_REG_OUTX_L_G                     = 0x22,
        LSM6DS3H_REG_OUTX_H_G                     = 0x23,
        LSM6DS3H_REG_OUTY_L_G                     = 0x24,
        LSM6DS3H_REG_OUTY_H_G                     = 0x25,
        LSM6DS3H_REG_OUTZ_L_G                     = 0x26,
        LSM6DS3H_REG_OUTZ_H_G                     = 0x27,

        LSM6DS3H_REG_OUTX_L_XL                    = 0x28,
        LSM6DS3H_REG_OUTX_H_XL                    = 0x29,
        LSM6DS3H_REG_OUTY_L_XL                    = 0x2a,
        LSM6DS3H_REG_OUTY_H_XL                    = 0x2b,
        LSM6DS3H_REG_OUTZ_L_XL                    = 0x2c,
        LSM6DS3H_REG_OUTZ_H_XL                    = 0x2d,

        LSM6DS3H_REG_SENSORHUB1_REG               = 0x2e,
        LSM6DS3H_REG_SENSORHUB2_REG               = 0x2f,
        LSM6DS3H_REG_SENSORHUB3_REG               = 0x30,
        LSM6DS3H_REG_SENSORHUB4_REG               = 0x31,
        LSM6DS3H_REG_SENSORHUB5_REG               = 0x32,
        LSM6DS3H_REG_SENSORHUB6_REG               = 0x33,
        LSM6DS3H_REG_SENSORHUB7_REG               = 0x34,
        LSM6DS3H_REG_SENSORHUB8_REG               = 0x35,
        LSM6DS3H_REG_SENSORHUB9_REG               = 0x36,
        LSM6DS3H_REG_SENSORHUB10_REG              = 0x37,
        LSM6DS3H_REG_SENSORHUB11_REG              = 0x38,
        LSM6DS3H_REG_SENSORHUB12_REG              = 0x39,

        LSM6DS3H_REG_FIFO_STATUS1                 = 0x3a,
        LSM6DS3H_REG_FIFO_STATUS2                 = 0x3b,
        LSM6DS3H_REG_FIFO_STATUS3                 = 0x3c,
        LSM6DS3H_REG_FIFO_STATUS4                 = 0x3d,

        LSM6DS3H_REG_FIFO_DATA_OUT_L              = 0x3e,
        LSM6DS3H_REG_FIFO_DATA_OUT_H              = 0x3f,

        LSM6DS3H_REG_TIMESTAMP0_REG               = 0x40,
        LSM6DS3H_REG_TIMESTAMP1_REG               = 0x41,
        LSM6DS3H_REG_TIMESTAMP2_REG               = 0x42,

        // 0x43-0x48 reserved

        LSM6DS3H_REG_STEP_TIMESTAMP_L             = 0x49,
        LSM6DS3H_REG_STEP_TIMESTAMP_H             = 0x4a,

        LSM6DS3H_REG_STEP_COUNTER_L               = 0x4b,
        LSM6DS3H_REG_STEP_COUNTER_H               = 0x4c,

        LSM6DS3H_REG_SENSORHUB13_REG              = 0x4d,
        LSM6DS3H_REG_SENSORHUB14_REG              = 0x4e,
        LSM6DS3H_REG_SENSORHUB15_REG              = 0x4f,
        LSM6DS3H_REG_SENSORHUB16_REG              = 0x50,
        LSM6DS3H_REG_SENSORHUB17_REG              = 0x51,
        LSM6DS3H_REG_SENSORHUB18_REG              = 0x52,

        LSM6DS3H_REG_FUNC_SRC                     = 0x53,

        // 0x54-0x57 reserved

        LSM6DS3H_REG_TAP_CFG                      = 0x58,
        LSM6DS3H_REG_TAP_THS_6D                   = 0x59,

        // where is int_dur1?
        LSM6DS3H_REG_INT_DUR2                     = 0x5a,

        LSM6DS3H_REG_WAKE_UP_THS                  = 0x5b,
        LSM6DS3H_REG_WAKE_UP_DUR                  = 0x5c,

        LSM6DS3H_REG_FREE_FALL                    = 0x5d,

        LSM6DS3H_REG_MD1_CFG                      = 0x5e,
        LSM6DS3H_REG_MD2_CFG                      = 0x5f,

        // 0x60-0x65 reserved

        LSM6DS3H_REG_OUT_MAG_RAW_X_L              = 0x66,
        LSM6DS3H_REG_OUT_MAG_RAW_X_H              = 0x67,
        LSM6DS3H_REG_OUT_MAG_RAW_Y_L              = 0x68,
        LSM6DS3H_REG_OUT_MAG_RAW_Y_H              = 0x69,
        LSM6DS3H_REG_OUT_MAG_RAW_Z_L              = 0x6a,
        LSM6DS3H_REG_OUT_MAG_RAW_Z_H              = 0x6b,

        // 0x6c-0x6f assume reserved, but not listed in DS

        LSM6DS3H_REG_CTRL_SPIAUX                  = 0x70,
    } LSM6DS3H_REGS_T;

    /**
     * REG_FUNC_CFG_ACCESS bits
     */
    typedef enum {
        // 0x00 - 0x40 reserved
        LSM6DS3H_FUNC_CFG_EN                      = 0x80,
    } LSM6DS3H_FUNC_CFG_ACCESS_BITS_T;

    /**
     * REG_ORIENT_CFG_G bits
     */
    typedef enum {
        // 0x00 - 0x40 reserved
        LSM6DS3H_ORIENT_CFG_G_ORIENT0             = 0x01,
        LSM6DS3H_ORIENT_CFG_G_ORIENT1             = 0x02,
        LSM6DS3H_ORIENT_CFG_G_ORIENT2             = 0x04,
        _LSM6DS3H_ORIENT_CFG_G_ORIENT_MASK        = 7,
        _LSM6DS3H_ORIENT_CFG_G_ORIENT_SHIFT       = 0,

        LSM6DS3H_ORIENT_CFG_G_SIGNZ               = 0x08,
        LSM6DS3H_ORIENT_CFG_G_SIGNY               = 0x10,
        LSM6DS3H_ORIENT_CFG_G_SIGNX               = 0x20,

        // 0x40-0x80 reserved
    } LSM6DS3H_ORIENT_CFG_G_BITS_T;

    /**
     * ORIENT_CFG_G_ORIENT values
     */
    typedef enum {
        LSM6DS3H_ORIENT_CFG_G_XYZ                 = 0,
        LSM6DS3H_ORIENT_CFG_G_XZY                 = 1,
        LSM6DS3H_ORIENT_CFG_G_YXZ                 = 2,
        LSM6DS3H_ORIENT_CFG_G_YZX                 = 3,
        LSM6DS3H_ORIENT_CFG_G_ZXY                 = 4,
        LSM6DS3H_ORIENT_CFG_G_ZYX                 = 5,
    } LSM6DS3H_ORIENT_G_ORIENT_T;

    /**
     * REG_INT1_CTRL bits
     */
    typedef enum {
        LSM6DS3H_INT1_CTRL_DRDY_XL                = 0x01,
        LSM6DS3H_INT1_CTRL_DRDY_G                 = 0x02,
        LSM6DS3H_INT1_CTRL_BOOT                   = 0x04,
        LSM6DS3H_INT1_CTRL_FTH                    = 0x08,
        LSM6DS3H_INT1_CTRL_FIFO_OVR               = 0x10,
        LSM6DS3H_INT1_CTRL_FULL_FLAG              = 0x20,
        LSM6DS3H_INT1_CTRL_SIGN_MOT               = 0x40,
        LSM6DS3H_INT1_CTRL_STEP_DETECTOR          = 0x80,
    } LSM6DS3H_INT1_CTRL_BITS_T;

    /**
     * REG_INT2_CTRL bits
     */
    typedef enum {
        LSM6DS3H_INT2_CTRL_DRDY_XL                = 0x01,
        LSM6DS3H_INT2_CTRL_DRDY_G                 = 0x02,
        LSM6DS3H_INT2_CTRL_DRDY_TEMP              = 0x04,
        LSM6DS3H_INT2_CTRL_FTH                    = 0x08,
        LSM6DS3H_INT2_CTRL_FIFO_OVR               = 0x10,
        LSM6DS3H_INT2_CTRL_FULL_FLAG              = 0x20,
        LSM6DS3H_INT2_CTRL_COUNT_OV               = 0x40,
        LSM6DS3H_INT2_CTRL_STEP_DELTA             = 0x80,
    } LSM6DS3H_INT2_CTRL_BITS_T;

    /**
     * REG_CTRL1_XL (accelerometer) bits
     */
    typedef enum {
        LSM6DS3H_CTRL1_XL_BW0                     = 0x01,
        LSM6DS3H_CTRL1_XL_BW1                     = 0x02,
        _LSM6DS3H_CTRL1_XL_BW_MASK                = 3,
        _LSM6DS3H_CTRL1_XL_BW_SHIFT               = 0,

        LSM6DS3H_CTRL1_XL_FS0                     = 0x04,
        LSM6DS3H_CTRL1_XL_FS1                     = 0x08,
        _LSM6DS3H_CTRL1_XL_FS_MASK                = 3,
        _LSM6DS3H_CTRL1_XL_FS_SHIFT               = 2,

        LSM6DS3H_CTRL1_XL_ODR0                    = 0x10,
        LSM6DS3H_CTRL1_XL_ODR1                    = 0x20,
        LSM6DS3H_CTRL1_XL_ODR2                    = 0x40,
        LSM6DS3H_CTRL1_XL_ODR3                    = 0x80,
        _LSM6DS3H_CTRL1_XL_ODR_MASK               = 15,
        _LSM6DS3H_CTRL1_XL_ODR_SHIFT              = 4,
    } LSM6DS3H_CTRL1_XL_BITS_T;

    /**
     * CTRL1_XL_BW values (anti-aliasing filter bw)
     */
    typedef enum {
        LSM6DS3H_XL_BW_400HZ                      = 0,
        LSM6DS3H_XL_BW_200HZ                      = 1,
        LSM6DS3H_XL_BW_100HZ                      = 2,
        LSM6DS3H_XL_BW_50HZ                       = 3,
    } LSM6DS3H_XL_BW_T;

    /**
     * CTRL1_XL_FS values (full scale)
     */
    typedef enum {
        LSM6DS3H_XL_FS_2G                         = 0,
        LSM6DS3H_XL_FS_16G                        = 1,
        LSM6DS3H_XL_FS_4G                         = 2,
        LSM6DS3H_XL_FS_8G                         = 3,
    } LSM6DS3H_XL_FS_T;

    /**
     * CTRL1_XL_ODR values (output data rate)
     */
    typedef enum {
        LSM6DS3H_XL_ODR_POWER_DOWN                = 0,
        LSM6DS3H_XL_ODR_12_5HZ                    = 1,
        LSM6DS3H_XL_ODR_26HZ                      = 2,
        LSM6DS3H_XL_ODR_52HZ                      = 3,
        LSM6DS3H_XL_ODR_104HZ                     = 4,
        LSM6DS3H_XL_ODR_208HZ                     = 5,
        LSM6DS3H_XL_ODR_416HZ                     = 6,
        LSM6DS3H_XL_ODR_833HZ                     = 7,
        LSM6DS3H_XL_ODR_1_66KHZ                   = 8,
        LSM6DS3H_XL_ODR_3_33KHZ                   = 9,
        LSM6DS3H_XL_ODR_6_66KHZ                   = 10,
    } LSM6DS3H_XL_ODR_T;

    /**
     * REG_CTRL2_G (gyroscope) bits
     */
    typedef enum {
        // 0x01 reserved

        LSM6DS3H_CTRL2_G_FS_125                   = 0x02,

        LSM6DS3H_CTRL2_G_FS0                      = 0x04,
        LSM6DS3H_CTRL2_G_FS1                      = 0x08,
        _LSM6DS3H_CTRL2_G_FS_MASK                 = 3,
        _LSM6DS3H_CTRL2_G_FS_SHIFT                = 2,

        LSM6DS3H_CTRL2_G_ODR0                     = 0x10,
        LSM6DS3H_CTRL2_G_ODR1                     = 0x20,
        LSM6DS3H_CTRL2_G_ODR2                     = 0x40,
        LSM6DS3H_CTRL2_G_ODR3                     = 0x80,
        _LSM6DS3H_CTRL2_G_ODR_MASK                = 0x15,
        _LSM6DS3H_CTRL2_G_ODR_SHIFT               = 0x4,
    } LSM6DS3H_CTRL2_G_BITS_T;

    /**
     * CTRL2_G_FS values (full scale)
     */
    typedef enum {
        LSM6DS3H_G_FS_245DPS                      = 0, // degrees per second
        LSM6DS3H_G_FS_500DPS                      = 1,
        LSM6DS3H_G_FS_1000DPS                     = 2,
        LSM6DS3H_G_FS_2000DPS                     = 3,

        // 125dps is a special case - it's just a bit you set or clear
        // to enable 125 or disable it.  We add a virtual bit 3 (4)
        // here as a flag to the driver to enable/disable this
        // "special" FS setting.
        LSM6DS3H_G_FS_125DPS                      = (4 + 0),
    } LSM6DS3H_G_FS_T;

    /**
     * CTRL2_G_ODR values (output data rate)
     */
    typedef enum {
        LSM6DS3H_G_ODR_POWER_DOWN                 = 0,
        LSM6DS3H_G_ODR_12_5HZ                     = 1,
        LSM6DS3H_G_ODR_26HZ                       = 2,
        LSM6DS3H_G_ODR_52HZ                       = 3,
        LSM6DS3H_G_ODR_104HZ                      = 4,
        LSM6DS3H_G_ODR_208HZ                      = 5,
        LSM6DS3H_G_ODR_416HZ                      = 6,
        LSM6DS3H_G_ODR_833HZ                      = 7,
        LSM6DS3H_G_ODR_1_66KHZ                    = 8,
    } LSM6DS3H_G_ODR_T;

    /**
     * REG_CTRL3 (_C, common) bits
     */
    typedef enum {
        LSM6DS3H_CTRL3_SW_RESET                   = 0x01,
        LSM6DS3H_CTRL3_BLE                        = 0x02,
        LSM6DS3H_CTRL3_IF_INC                     = 0x04,
        LSM6DS3H_CTRL3_SIM                        = 0x08,
        LSM6DS3H_CTRL3_PP_OD                      = 0x10,
        LSM6DS3H_CTRL3_H_LACTIVE                  = 0x20,
        LSM6DS3H_CTRL3_BDU                        = 0x40,
        LSM6DS3H_CTRL3_BOOT                       = 0x80,
    } LSM6DS3H_CTRL3_BITS_T;

    /**
     * REG_CTRL4 (_C, common) bits
     */
    typedef enum {
        LSM6DS3H_CTRL4_STOP_ON_FTH                = 0x01,
        LSM6DS3H_CTRL4_3_3KHZ_ODR                 = 0x02,
        LSM6DS3H_CTRL4_I2C_DISABLE                = 0x04,
        LSM6DS3H_CTRL4_DRDY_MASK                  = 0x08,
        LSM6DS3H_CTRL4_FIFO_TEMP_EN               = 0x10,
        LSM6DS3H_CTRL4_INT2_ON_INT1               = 0x20,
        LSM6DS3H_CTRL4_SLEEP_G                    = 0x40,
        LSM6DS3H_CTRL4_XL_BW_SCAL_ODR             = 0x80,
    } LSM6DS3H_CTRL4_BITS_T;

    /**
     * REG_CTRL5 (_C, common) bits
     */
    typedef enum {
        LSM6DS3H_CTRL5_ST_XL0                     = 0x01,
        LSM6DS3H_CTRL5_ST_XL1                     = 0x02,
        _LSM6DS3H_CTRL5_ST_XL_MASK                = 3,
        _LSM6DS3H_CTRL5_ST_XL_SHIFT               = 0,

        LSM6DS3H_CTRL5_ST_G0                      = 0x04,
        LSM6DS3H_CTRL5_ST_G1                      = 0x08,
        _LSM6DS3H_CTRL5_ST_G_MASK                 = 3,
        _LSM6DS3H_CTRL5_ST_G_SHIFT                = 2,

        // 0x10 reserved

        LSM6DS3H_CTRL5_ROUNDING0                  = 0x20,
        LSM6DS3H_CTRL5_ROUNDING1                  = 0x40,
        LSM6DS3H_CTRL5_ROUNDING2                  = 0x80,
        _LSM6DS3H_CTRL5_ROUNDING_MASK             = 7,
        _LSM6DS3H_CTRL5_ROUNDING_SHIFT            = 5,
    } LSM6DS3H_CTRL5_BITS_T;

    /**
     * CTRL5_ST_XL values (self test accelerometer)
     */
    typedef enum {
        LSM6DS3H_ST_XL_NORMAL                     = 0,
        LSM6DS3H_ST_XL_POSITIVE                   = 1,
        LSM6DS3H_ST_XL_NEGATIVE                   = 2,
    } LSM6DS3H_ST_XL_T;

    /**
     * CTRL5_ST_G values (self test gyroscope)
     */
    typedef enum {
        LSM6DS3H_ST_G_NORMAL                     = 0,
        LSM6DS3H_ST_G_POSITIVE                   = 1,
        LSM6DS3H_ST_G_NEGATIVE                   = 3,
    } LSM6DS3H_ST_G_T;

    /**
     * REG_CTRL6 (_C, common) bits
     */
    typedef enum {
        // 0x01-0x08 reserved
        LSM6DS3H_CTRL6_XL_HM_MODE                = 0x10,
        LSM6DS3H_CTRL6_LVL2_EN                   = 0x20,
        LSM6DS3H_CTRL6_LVLEN                     = 0x40,
        LSM6DS3H_CTRL6_TRIG_EN                   = 0x80,
    } LSM6DS3H_CTRL6_BITS_T;

    /**
     * REG_CTRL7_G bits
     */
    typedef enum {
        // 0x01-0x02 reserved
        LSM6DS3H_CTRL7_G_ROUNDING_STATUS         = 0x04,
        LSM6DS3H_CTRL7_G_HP_RST                  = 0x08,

        LSM6DS3H_CTRL7_G_HPCF0                   = 0x10,
        LSM6DS3H_CTRL7_G_HPCF1                   = 0x20,
        _LSM6DS3H_CTRL7_G_HPCF_MASK              = 3,
        _LSM6DS3H_CTRL7_G_HPCF_SHIFT             = 4,

        LSM6DS3H_CTRL7_G_HP_EN                   = 0x40,
        LSM6DS3H_CTRL7_G_HM_MODE                 = 0x80,
    } LSM6DS3H_CTRL7_G_BITS_T;

    /**
     * CTRL7_G_HPCF values (high pass cutoff, in high performance mode only)
     */
    typedef enum {
        LSM6DS3H_G_HPCF_0_0081HZ                 = 0, // 0.0081hz
        LSM6DS3H_G_HPCF_0_0324HZ                 = 1,
        LSM6DS3H_G_HPCF_2_07HZ                   = 2,
        LSM6DS3H_G_HPCF_16_32HZ                  = 3,
    } LSM6DS3H_G_HPCF_T;

    /**
     * REG_CTRL8_XL bits
     */
    typedef enum {
        LSM6DS3H_CTRL8_XL_LOW_PASS_ON_6D         = 0x01,

        // 0x02 reserved

        LSM6DS3H_CTRL8_XL_HP_SLOPE_EN            = 0x04,

        // 0x08-0x10 reserved

        LSM6DS3H_CTRL8_XL_HPCF0                  = 0x20,
        LSM6DS3H_CTRL8_XL_HPCF1                  = 0x40,
        _LSM6DS3H_CTRL8_XL_HPCF_MASK             = 3,
        _LSM6DS3H_CTRL8_XL_HPCF_SHIFT            = 5,

        LSM6DS3H_CTRL8_XL_LPF2_EN                = 0x80,
    } LSM6DS3H_CTRL8_XL_BITS_T;

    /**
     * REG_CTRL9_XL bits
     */
    typedef enum {
        // 0x01-0x02 reserved

        LSM6DS3H_CTRL9_XL_SOFT_EN                = 0x04,
        LSM6DS3H_CTRL9_XL_XEN                    = 0x08,
        LSM6DS3H_CTRL9_XL_YEN                    = 0x10,
        LSM6DS3H_CTRL9_XL_ZEN                    = 0x20,

        // 0x40-0x80 reserved
    } LSM6DS3H_CTRL9_XL_BITS_T;

    /**
     * REG_CTRL10_C bits
     */
    typedef enum {
        LSM6DS3H_CTRL10_C_SIGN_MOT_EN            = 0x01,
        LSM6DS3H_CTRL10_C_PEDO_RST_STEP          = 0x02,
        LSM6DS3H_CTRL10_C_FUNC_EN                = 0x04,
        LSM6DS3H_CTRL10_C_XEN                    = 0x08,
        LSM6DS3H_CTRL10_C_YEN                    = 0x10,
        LSM6DS3H_CTRL10_C_ZEN                    = 0x20,

        // 0x40-0x80 reserved
    } LSM6DS3H_CTRL10_C_BITS_T;

    /**
     * REG_MASTER_CONFIG bits
     */
    typedef enum {
        LSM6DS3H_MASTER_CONFIG_MASTER_ON         = 0x01,
        LSM6DS3H_MASTER_CONFIG_IRON_EN           = 0x02,
        LSM6DS3H_MASTER_CONFIG_PASS_THROUGH_MODE = 0x04,
        LSM6DS3H_MASTER_CONFIG_PULL_UP_EN        = 0x08,
        LSM6DS3H_MASTER_CONFIG_START_CONFIG      = 0x10,

        // 0x20 reserved

        LSM6DS3H_MASTER_CONFIG_DATA_VALID_SEL_FIFO = 0x40,
        LSM6DS3H_MASTER_CONFIG_DRDY_ON_INT1      = 0x80,
    } LSM6DS3H_MASTER_CONFIG_BITS_T;

    /**
     * REG_WAKE_UP_SRC bits
     */
    typedef enum {
        LSM6DS3H_WAKE_UP_SRC_Z_WU                = 0x01,
        LSM6DS3H_WAKE_UP_SRC_Y_WU                = 0x02,
        LSM6DS3H_WAKE_UP_SRC_X_WU                = 0x04,
        LSM6DS3H_WAKE_UP_SRC_WU_IA               = 0x08,
        LSM6DS3H_WAKE_UP_SRC_SLEEP_STATE_IA      = 0x10,
        LSM6DS3H_WAKE_UP_SRC_FF_AA               = 0x20,

        // 0x40-0x80 reserved
    } LSM6DS3H_WAKE_UP_SRC_BITS_T;

    /**
     * REG_TAP_SRC bits
     */
    typedef enum {
        LSM6DS3H_TAP_SRC_Z_TAP                   = 0x01,
        LSM6DS3H_TAP_SRC_Y_TAP                   = 0x02,
        LSM6DS3H_TAP_SRC_X_TAP                   = 0x04,
        LSM6DS3H_TAP_SRC_TAP_SIGN                = 0x08,
        LSM6DS3H_TAP_SRC_DOUBLE_TAP              = 0x10,
        LSM6DS3H_TAP_SRC_SINGLE_TAP              = 0x20,
        LSM6DS3H_TAP_SRC_TAP_IA                  = 0x40,

        // 0x80 reserved
    } LSM6DS3H_TAP_SRC_BITS_T;

    /**
     * REG_D6D_SRC bits
     */
    typedef enum {
        LSM6DS3H_D6D_SRC_XL                      = 0x01,
        LSM6DS3H_D6D_SRC_XH                      = 0x02,
        LSM6DS3H_D6D_SRC_YL                      = 0x04,
        LSM6DS3H_D6D_SRC_YH                      = 0x08,
        LSM6DS3H_D6D_SRC_ZL                      = 0x10,
        LSM6DS3H_D6D_SRC_ZH                      = 0x20,
        LSM6DS3H_D6D_SRC_D6D_IA                  = 0x40,

        // 0x80 reserved
    } LSM6DS3H_D6D_SRC_BITS_T;

    /**
     * REG_STATUS bits
     */
    typedef enum {
        LSM6DS3H_STATUS_XLDA                     = 0x01, // acc data avail
        LSM6DS3H_STATUS_GDA                      = 0x02, // gyr data avail
        LSM6DS3H_STATUS_TDA                      = 0x04, // temp data avail

        // 0x08-0x80 reserved
    } LSM6DS3H_STATUS_BITS_T;

    /**
     * REG_FUNC_SRC bits
     */
    typedef enum {
        LSM6DS3H_FUNC_SRC_SENSORHUB_END_OP       = 0x01,
        LSM6DS3H_FUNC_SRC_SI_END_OP              = 0x02,

        // 0x04 reserved

        LSM6DS3H_FUNC_SRC_STEP_OVERFLOW          = 0x08,
        LSM6DS3H_FUNC_SRC_STEP_DETECTED          = 0x10,
        LSM6DS3H_FUNC_SRC_TILT_IA                = 0x20,
        LSM6DS3H_FUNC_SRC_SIGN_MOTION_IA         = 0x40,
        LSM6DS3H_FUNC_SRC_STEP_COUNT_DELTA_IA    = 0x80,
    } LSM6DS3H_FUNC_SRC_BITS_T;

    /**
     * REG_MD1_CFG bits (function routing to INT1).  We are omitting
     * the "INT1" redundancy in these item names.
     */
    typedef enum {
        LSM6DS3H_MD1_CFG_TIMER                   = 0x01,
        LSM6DS3H_MD1_CFG_TILT                    = 0x02,
        LSM6DS3H_MD1_CFG_6D                      = 0x04,
        LSM6DS3H_MD1_CFG_DOUBLE_TAP              = 0x08,
        LSM6DS3H_MD1_CFG_FF                      = 0x10,
        LSM6DS3H_MD1_CFG_WU                      = 0x20,
        LSM6DS3H_MD1_CFG_SINGLE_TAP              = 0x40,
        LSM6DS3H_MD1_CFG_INACT_STATE             = 0x80,
    } LSM6DS3H_MD1_CFG_BITS_T;

    /**
     * REG_MD2_CFG bits (function routing to INT2).  We are omitting
     * the "INT2" redundancy in these item names.
     */
    typedef enum {
        LSM6DS3H_MD2_CFG_IRON                    = 0x01,
        LSM6DS3H_MD2_CFG_TILT                    = 0x02,
        LSM6DS3H_MD2_CFG_6D                      = 0x04,
        LSM6DS3H_MD2_CFG_DOUBLE_TAP              = 0x08,
        LSM6DS3H_MD2_CFG_FF                      = 0x10,
        LSM6DS3H_MD2_CFG_WU                      = 0x20,
        LSM6DS3H_MD2_CFG_SINGLE_TAP              = 0x40,
        LSM6DS3H_MD2_CFG_INACT_STATE             = 0x80,
    } LSM6DS3H_MD2_CFG_BITS_T;

    // interrupt selection for installISR() and uninstallISR()
    typedef enum {
        LSM6DS3H_INTERRUPT_INT1,
        LSM6DS3H_INTERRUPT_INT2
    } LSM6DS3H_INTERRUPT_PINS_T;


#ifdef __cplusplus
}
#endif
