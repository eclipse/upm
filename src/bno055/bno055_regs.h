/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016-2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#define BNO055_DEFAULT_I2C_BUS 0
#define BNO055_DEFAULT_ADDR    0x28

// The chip ID, for verification in bno055_init().
#define BNO055_CHIPID          0xa0

// number of bytes of stored calibration data
#define BNO055_CALIBRATION_DATA_SIZE (22)

#ifdef __cplusplus
extern "C" {
#endif

    // NOTE: Reserved registers should not be written into.  Reading
    // from them will return indeterminate values.
    //
    // The register map is divided into two pages - page 1 contains
    // sensor specific configuration registers, and page 0 contains all
    // other configuration data and sensor output registers.

    /**
     * BNO055 registers
     */
    typedef enum {
        // The first register listed here is the page ID register.  It
        // is the same on both pages, and selects or indicates the
        // currently active register page.

        BNO055_REG_PAGE_ID                      = 0x07,

        // Page 0
        BNO055_REG_CHIP_ID                      = 0x00,
        BNO055_REG_ACC_ID                       = 0x01, // accel id
        BNO055_REG_MAG_ID                       = 0x02, // mag id
        BNO055_REG_GYR_ID                       = 0x03, // gyro id
        BNO055_REG_SW_REV_ID_LSB                = 0x04,
        BNO055_REG_SW_REV_ID_MSB                = 0x05,
        BNO055_REG_BL_REV_ID                    = 0x06, // bootloader rev

        BNO055_REG_ACC_DATA_X_LSB               = 0x08,
        BNO055_REG_ACC_DATA_X_MSB               = 0x09,
        BNO055_REG_ACC_DATA_Y_LSB               = 0x0a,
        BNO055_REG_ACC_DATA_Y_MSB               = 0x0b,
        BNO055_REG_ACC_DATA_Z_LSB               = 0x0c,
        BNO055_REG_ACC_DATA_Z_MSB               = 0x0d,

        BNO055_REG_MAG_DATA_X_LSB               = 0x0e,
        BNO055_REG_MAG_DATA_X_MSB               = 0x0f,
        BNO055_REG_MAG_DATA_Y_LSB               = 0x10,
        BNO055_REG_MAG_DATA_Y_MSB               = 0x11,
        BNO055_REG_MAG_DATA_Z_LSB               = 0x12,
        BNO055_REG_MAG_DATA_Z_MSB               = 0x13,

        BNO055_REG_GYR_DATA_X_LSB               = 0x14,
        BNO055_REG_GYR_DATA_X_MSB               = 0x15,
        BNO055_REG_GYR_DATA_Y_LSB               = 0x16,
        BNO055_REG_GYR_DATA_Y_MSB               = 0x17,
        BNO055_REG_GYR_DATA_Z_LSB               = 0x18,
        BNO055_REG_GYR_DATA_Z_MSB               = 0x19,

        // euler angles
        BNO055_REG_EUL_HEADING_LSB              = 0x1a,
        BNO055_REG_EUL_HEADING_MSB              = 0x1b,
        BNO055_REG_EUL_ROLL_LSB                 = 0x1c,
        BNO055_REG_EUL_ROLL_MSB                 = 0x1d,
        BNO055_REG_EUL_PITCH_LSB                = 0x1e,
        BNO055_REG_EUL_PITCH_MSB                = 0x1f,

        // Quaternions
        BNO055_REG_QUA_DATA_W_LSB               = 0x20,
        BNO055_REG_QUA_DATA_W_MSB               = 0x21,
        BNO055_REG_QUA_DATA_X_LSB               = 0x22,
        BNO055_REG_QUA_DATA_X_MSB               = 0x23,
        BNO055_REG_QUA_DATA_Y_LSB               = 0x24,
        BNO055_REG_QUA_DATA_Y_MSB               = 0x25,
        BNO055_REG_QUA_DATA_Z_LSB               = 0x26,
        BNO055_REG_QUA_DATA_Z_MSB               = 0x27,

        // linear accel data
        BNO055_REG_LIA_DATA_X_LSB               = 0x28,
        BNO055_REG_LIA_DATA_X_MSB               = 0x29,
        BNO055_REG_LIA_DATA_Y_LSB               = 0x2a,
        BNO055_REG_LIA_DATA_Y_MSB               = 0x2b,
        BNO055_REG_LIA_DATA_Z_LSB               = 0x2c,
        BNO055_REG_LIA_DATA_Z_MSB               = 0x2d,

        // gravity vector
        BNO055_REG_GRV_DATA_X_LSB               = 0x2e,
        BNO055_REG_GRV_DATA_X_MSB               = 0x2f,
        BNO055_REG_GRV_DATA_Y_LSB               = 0x30,
        BNO055_REG_GRV_DATA_Y_MSB               = 0x31,
        BNO055_REG_GRV_DATA_Z_LSB               = 0x32,
        BNO055_REG_GRV_DATA_Z_MSB               = 0x33,

        BNO055_REG_TEMPERATURE                  = 0x34,

        BNO055_REG_CALIB_STAT                   = 0x35, // calibration status
        BNO055_REG_ST_RESULT                    = 0x36, // selftest result

        BNO055_REG_INT_STA                      = 0x37, // interrupt status

        BNO055_REG_SYS_CLK_STATUS               = 0x38,

        BNO055_REG_SYS_STATUS                   = 0x39,
        BNO055_REG_SYS_ERROR                    = 0x3a,

        BNO055_REG_UNIT_SEL                     = 0x3b,

        // 0x3c reserved

        BNO055_REG_OPER_MODE                    = 0x3d, // operating mode
        BNO055_REG_POWER_MODE                   = 0x3e,

        BNO055_REG_SYS_TRIGGER                  = 0x3f,
        BNO055_REG_TEMP_SOURCE                  = 0x40, // temperature src

        BNO055_REG_AXIS_MAP_CONFIG              = 0x41,
        BNO055_REG_AXIS_MAP_SIGN                = 0x42,

        // 0x43-0x54 reserved

        // stored calibration data
        BNO055_REG_ACC_OFFSET_X_LSB             = 0x55,
        BNO055_REG_ACC_OFFSET_X_MSB             = 0x56,
        BNO055_REG_ACC_OFFSET_Y_LSB             = 0x57,
        BNO055_REG_ACC_OFFSET_Y_MSB             = 0x58,
        BNO055_REG_ACC_OFFSET_Z_LSB             = 0x59,
        BNO055_REG_ACC_OFFSET_Z_MSB             = 0x5a,

        BNO055_REG_MAG_OFFSET_X_LSB             = 0x5b,
        BNO055_REG_MAG_OFFSET_X_MSB             = 0x5c,
        BNO055_REG_MAG_OFFSET_Y_LSB             = 0x5d,
        BNO055_REG_MAG_OFFSET_Y_MSB             = 0x5e,
        BNO055_REG_MAG_OFFSET_Z_LSB             = 0x5f,
        BNO055_REG_MAG_OFFSET_Z_MSB             = 0x60,

        BNO055_REG_GYR_OFFSET_X_LSB             = 0x61,
        BNO055_REG_GYR_OFFSET_X_MSB             = 0x62,
        BNO055_REG_GYR_OFFSET_Y_LSB             = 0x63,
        BNO055_REG_GYR_OFFSET_Y_MSB             = 0x64,
        BNO055_REG_GYR_OFFSET_Z_LSB             = 0x65,
        BNO055_REG_GYR_OFFSET_Z_MSB             = 0x66,

        BNO055_REG_ACC_RADIUS_LSB               = 0x67,
        BNO055_REG_ACC_RADIUS_MSB               = 0x68,

        BNO055_REG_MAG_RADIUS_LSB               = 0x69,
        BNO055_REG_MAG_RADIUS_MSB               = 0x6a,

        // 0x6b-0x7f reserved
        // end of page 0

        // Page 1

        // 0x00-0x06 reserved
        // 0x07 - page id

        BNO055_REG_ACC_CONFIG                   = 0x08,
        BNO055_REG_MAG_CONFIG                   = 0x09,
        BNO055_REG_GYR_CONFIG0                  = 0x0a,
        BNO055_REG_GYR_CONFIG1                  = 0x0b,
        BNO055_REG_ACC_SLEEP_CONFIG             = 0x0c,
        BNO055_REG_GYR_SLEEP_CONFIG             = 0x0d,

        // 0x0e reserved
        BNO055_REG_INT_MSK                      = 0x0f,
        BNO055_REG_INT_EN                       = 0x10,

        BNO055_REG_ACC_AM_THRES                 = 0x11,
        BNO055_REG_ACC_INT_SETTINGS             = 0x12,
        BNO055_REG_ACC_HG_DURATION              = 0x13,
        BNO055_REG_ACC_HG_THRES                 = 0x14,
        BNO055_REG_ACC_NM_THRES                 = 0x15,
        BNO055_REG_ACC_NM_SET                   = 0x16,

        BNO055_REG_GYR_INT_SETTING              = 0x17,
        BNO055_REG_GYR_HR_X_SET                 = 0x18,
        BNO055_REG_GYR_DUR_X                    = 0x19,
        BNO055_REG_GYR_HR_Y_SET                 = 0x1a,
        BNO055_REG_GYR_DUR_Y                    = 0x1b,
        BNO055_REG_GYR_HR_Z_SET                 = 0x1c,
        BNO055_REG_GYR_DUR_Z                    = 0x1d,
        BNO055_REG_GYR_AM_THRES                 = 0x1e,
        BNO055_REG_GYR_AM_SET                   = 0x1f,

        // 0x20-0x4f reserved

        // 16 byte (0x50-0x5f) unique ID
        BNO055_REG_BNO_UNIQUE_ID                = 0x50

        // 0x60-0x7f reserved
    } BNO055_REGS_T;

    // Page 0 register enumerants

    /**
     * BNO055_REG_CALIB_STAT bits
     */
    typedef enum {
        BNO055_CALIB_STAT_MAG0                  = 0x01,
        BNO055_CALIB_STAT_MAG1                  = 0x02,
        _BNO055_CALIB_STAT_MAG_MASK             = 3,
        _BNO055_CALIB_STAT_MAG_SHIFT            = 0,

        BNO055_CALIB_STAT_ACC0                  = 0x04,
        BNO055_CALIB_STAT_ACC1                  = 0x08,
        _BNO055_CALIB_STAT_ACC_MASK             = 3,
        _BNO055_CALIB_STAT_ACC_SHIFT            = 2,

        BNO055_CALIB_STAT_GYR0                  = 0x10,
        BNO055_CALIB_STAT_GYR1                  = 0x20,
        _BNO055_CALIB_STAT_GYR_MASK             = 3,
        _BNO055_CALIB_STAT_GYR_SHIFT            = 4,

        BNO055_CALIB_STAT_SYS0                  = 0x40,
        BNO055_CALIB_STAT_SYS1                  = 0x80,
        _BNO055_CALIB_STAT_SYS_MASK             = 3,
        _BNO055_CALIB_STAT_SYS_SHIFT            = 6
    } BNO055_CALIB_STAT_BITS_T;

    /**
     * BNO055_REG_ST_RESULT bits
     */
    typedef enum {
        BNO055_ST_RESULT_ACC                    = 0x01,
        BNO055_ST_RESULT_MAG                    = 0x02,
        BNO055_ST_RESULT_GYR                    = 0x04,
        BNO055_ST_RESULT_MCU                    = 0x08
        // 0x10-0x80 reserved
    } BNO055_ST_RESULT_BITS_T;

    /**
     * BNO055_REG_INT_STA bits
     */
    typedef enum {
        // 0x01-0x02 reserved
        BNO055_INT_STA_GYRO_AM                  = 0x04, // gyro any-motion
        BNO055_INT_STA_GYR_HIGH_RATE            = 0x08,
        // 0x010 reserved
        BNO055_INT_STA_ACC_HIGH_G               = 0x20,
        BNO055_INT_STA_ACC_AM                   = 0x40, // accel any-motion
        BNO055_INT_STA_ACC_NM                   = 0x80  // accel no-motion
    } BNO055_INT_STA_BITS_T;

    /**
     * BNO055_REG_SYS_CLK_STATUS bits
     */
    typedef enum {
        BNO055_SYS_CLK_STATUS_ST_MAIN_CLK       = 0x01
        // 0x02-0x80 reserved
    } BNO055_SYS_CLK_STATUS_BITS_T;

    /**
     * BNO055_REG_SYS_STATUS values
     */
    typedef enum {
        BNO055_SYS_STATUS_IDLE                   = 0,
        BNO055_SYS_STATUS_SYS_ERR                = 1,
        BNO055_SYS_STATUS_INIT_PERIPHERALS       = 2,
        BNO055_SYS_STATUS_SYSTEM_INIT            = 3,
        BNO055_SYS_STATUS_EXECUTING_SELFTEST     = 4,
        BNO055_SYS_STATUS_FUSION_RUNNING         = 5,
        BNO055_SYS_STATUS_NO_FUSION_RUNNING      = 6
    } BNO055_SYS_STATUS_T;

    /**
     * BNO055_REG_SYS_ERR values
     */
    typedef enum {
        BNO055_SYS_ERR_NOERROR                   = 0,
        BNO055_SYS_ERR_PERIPH_INIT_ERROR         = 1,
        BNO055_SYS_ERR_SYS_INIT_ERROR            = 2,
        BNO055_SYS_ERR_SELFTEST_FAIL_ERROR       = 3,
        BNO055_SYS_ERR_REG_VAL_OUTOFRANGE_ERROR  = 4,
        BNO055_SYS_ERR_REG_ADDR_OUTOFRANGE_ERROR = 5,
        BNO055_SYS_ERR_REG_WRITE_ERROR           = 6,
        BNO055_SYS_ERR_LP_MODE_NOT_AVAIL_ERROR   = 7,
        BNO055_SYS_ERR_ACC_PWR_MODE_NOT_AVAIL_ERROR = 8,
        BNO055_SYS_ERR_FUSION_CONFIG_ERROR       = 9,
        BNO055_SYS_ERR_SENSOR_CONFIG_ERROR       = 10
    } BNO055_SYS_ERR_T;


    /**
     * BNO055_REG_UNIT_SEL bits
     */
    typedef enum {
        BNO055_UNIT_SEL_ACC_UNIT                = 0x01, // 0=m/s^2, 1=mg
        BNO055_UNIT_SEL_GYR_UNIT                = 0x02, // 0=dps, 1=rps
        BNO055_UNIT_SEL_EUL_UNIT                = 0x04, // 0=degrees, 1=radians
        // 0x08 reserved
        BNO055_UNIT_SEL_TEMP_UNIT               = 0x10, // 0=C, 1=F
        // 0x20-0x40 reserved
        BNO055_UNIT_SEL_ORI_ANDROID_WINDOWS     = 0x80  // 0=windows
                                                        // orient,
                                                        // 1=android
    } BNO055_UNIT_SEL_BITS_T;

    /**
     * BNO055_REG_OPR_MODE bits
     */
    typedef enum {
        BNO055_OPR_MODE_OPERATION_MODE0         = 0x01,
        BNO055_OPR_MODE_OPERATION_MODE1         = 0x02,
        BNO055_OPR_MODE_OPERATION_MODE2         = 0x04,
        BNO055_OPR_MODE_OPERATION_MODE3         = 0x08,
        _BNO055_OPR_MODE_OPERATION_MODE_MASK    = 15,
        _BNO055_OPR_MODE_OPERATION_MODE_SHIFT   = 0
        // 0x10-0x80 reserved
    } BNO055_OPR_MODE_BITS_T;

    /**
     * BNO055_OPR_MODE_OPERATION values
     */
    typedef enum {
        BNO055_OPERATION_MODE_CONFIGMODE        = 0,
        BNO055_OPERATION_MODE_ACCONLY           = 1,
        BNO055_OPERATION_MODE_MAGONLY           = 2,
        BNO055_OPERATION_MODE_GYROONLY          = 3,
        BNO055_OPERATION_MODE_ACCMAG            = 4,
        BNO055_OPERATION_MODE_ACCGYRO           = 5,
        BNO055_OPERATION_MODE_MAGGYRO           = 6,
        BNO055_OPERATION_MODE_AMG               = 7,
        // fusion modes
        BNO055_OPERATION_MODE_IMU               = 8,
        BNO055_OPERATION_MODE_COMPASS           = 9,
        BNO055_OPERATION_MODE_M4G               = 10,
        BNO055_OPERATION_MODE_NDOF_FMC_OFF      = 11,
        BNO055_OPERATION_MODE_NDOF              = 12
    } BNO055_OPERATION_MODES_T;

    /**
     * BNO055_REG_PWR_MODE bits
     */
    typedef enum {
        BNO055_PWR_MODE_POWER_MODE0             = 0x01,
        BNO055_PWR_MODE_POWER_MODE1             = 0x02,
        _BNO055_PWR_MODE_POWER_MODE_MASK        = 3,
        _BNO055_PWR_MODE_POWER_MODE_SHIFT       = 0
        // 0x04-0x80 reserved
    } BNO055_PWR_MODE_BITS_T;

    /**
     * BNO055_POWER_MODE values
     */
    typedef enum {
        BNO055_POWER_MODE_NORMAL                = 0,
        BNO055_POWER_MODE_LOW                   = 1,
        BNO055_POWER_MODE_SUSPEND               = 2
    } POWER_MODES_T;

    /**
     * BNO055_REG_SYS_TRIGGER bits
     */
    typedef enum {
        BNO055_SYS_TRIGGER_SELF_TEST            = 0x01,
        // 0x02-0x10 reserved
        BNO055_SYS_TRIGGER_RST_SYS              = 0x20,
        BNO055_SYS_TRIGGER_RST_INT              = 0x40,
        BNO055_SYS_TRIGGER_CLK_SEL              = 0x80
    } BNO055_SYS_TRIGGER_BITS_T;

    /**
     * BNO055_REG_TEMP_SOURCE bits
     */
    typedef enum {
        BNO055_TEMP_SOURCE_TEMP_SOURCE0         = 0x01,
        BNO055_TEMP_SOURCE_TEMP_SOURCE1         = 0x02,
        _BNO055_TEMP_SOURCE_TEMP_SOURCE_MASK    = 3,
        _BNO055_TEMP_SOURCE_TEMP_SOURCE_SHIFT   = 0
        // 0x04-0x80 reserved
    } BNO055_TEMP_SOURCE_BITS_T;

    /**
     * BNO055_TEMP_SOURCE values
     */
    typedef enum {
        BNO055_TEMP_SOURCE_ACC                  = 0,
        BNO055_TEMP_SOURCE_GYR                  = 1
    } BNO055_TEMP_SOURCES_T;

    /**
     * BNO055_REG_AXIS_MAP_CONFIG bits
     */
    typedef enum {
        BNO055_AXIS_MAP_CONFIG_REMAPPED_X_VAL0  = 0x01,
        BNO055_AXIS_MAP_CONFIG_REMAPPED_X_VAL1  = 0x02,
        _BNO055_AXIS_MAP_CONFIG_REMAPPED_X_VAL_MASK = 3,
        _BNO055_AXIS_MAP_CONFIG_REMAPPED_X_VAL_SHIFT = 0,

        BNO055_AXIS_MAP_CONFIG_REMAPPED_Y_VAL0  = 0x04,
        BNO055_AXIS_MAP_CONFIG_REMAPPED_Y_VAL1  = 0x08,
        _BNO055_AXIS_MAP_CONFIG_REMAPPED_Y_VAL_MASK = 3,
        _BNO055_AXIS_MAP_CONFIG_REMAPPED_Y_VAL_SHIFT = 2,

        BNO055_AXIS_MAP_CONFIG_REMAPPED_Z_VAL0  = 0x10,
        BNO055_AXIS_MAP_CONFIG_REMAPPED_Z_VAL1  = 0x20,
        _BNO055_AXIS_MAP_CONFIG_REMAPPED_Z_VAL_MASK = 3,
        _BNO055_AXIS_MAP_CONFIG_REMAPPED_Z_VAL_SHIFT = 4
        // 0x40-0x80 reserved
    } BNO055_AXIS_MAP_CONFIG_BITS_T;

    /**
     * REMAPPED_AXIS values, applied to X, Y, and Z axes
     * (BNO055_REG_AXIS_MAP_CONFIG)
     */
    typedef enum {
        BNO055_REMAPPED_AXIS_X                  = 0,
        BNO055_REMAPPED_AXIS_Y                  = 1,
        BNO055_REMAPPED_AXIS_Z                  = 2
    } BNO055_REMAPPED_AXIS_T;

    /**
     * BNO055_REG_AXIS_MAP_SIGN bits
     */
    typedef enum {
        BNO055_AXIS_MAP_SIGN_REMAPPED_Z_SIGN    = 0x01,
        BNO055_AXIS_MAP_SIGN_REMAPPED_Y_SIGN    = 0x02,
        BNO055_AXIS_MAP_SIGN_REMAPPED_X_SIGN    = 0x04
        // 0x08-0x80 reserved
    } BNO055_AXIS_MAP_SIGN_BITS_T;

    // Page 1 register enumerants

    /**
     * BNO055_REG_ACC_CONFIG bits
     */
    typedef enum {
        BNO055_ACC_CONFIG_ACC_RANGE0            = 0x01,
        BNO055_ACC_CONFIG_ACC_RANGE1            = 0x02,
        _BNO055_ACC_CONFIG_ACC_RANGE_MASK       = 3,
        _BNO055_ACC_CONFIG_ACC_RANGE_SHIFT      = 0,

        BNO055_ACC_CONFIG_ACC_BW0               = 0x04,
        BNO055_ACC_CONFIG_ACC_BW1               = 0x08,
        BNO055_ACC_CONFIG_ACC_BW2               = 0x10,
        _BNO055_ACC_CONFIG_ACC_BW_MASK          = 7,
        _BNO055_ACC_CONFIG_ACC_BW_SHIFT         = 2,

        BNO055_ACC_CONFIG_ACC_PWR_MODE0         = 0x20,
        BNO055_ACC_CONFIG_ACC_PWR_MODE1         = 0x40,
        BNO055_ACC_CONFIG_ACC_PWR_MODE2         = 0x80,
        _BNO055_ACC_CONFIG_ACC_PWR_MODE_MASK    = 7,
        _BNO055_ACC_CONFIG_ACC_PWR_MODE_SHIFT   = 5
    } BNO055_ACC_CONFIG_BITS_T;

    /**
     * BNO055_ACC_CONFIG_ACC_RANGE values
     */
    typedef enum {
        BNO055_ACC_RANGE_2G                     = 0,
        BNO055_ACC_RANGE_4G                     = 1,
        BNO055_ACC_RANGE_8G                     = 2,
        BNO055_ACC_RANGE_16G                    = 3
    } BNO055_ACC_RANGE_T;

    /**
     * BNO055_ACC_CONFIG_ACC_BW values
     */
    typedef enum {
        BNO055_ACC_BW_7_81                      = 0, // 7.81 Hz
        BNO055_ACC_BW_15_53                     = 1,
        BNO055_ACC_BW_31_25                     = 2,
        BNO055_ACC_BW_62_5                      = 3,
        BNO055_ACC_BW_125                       = 4, // 125 Hz
        BNO055_ACC_BW_250                       = 5,
        BNO055_ACC_BW_500                       = 6,
        BNO055_ACC_BW_1000                      = 7
    } BNO055_ACC_BW_T;

    /**
     * BNO055_ACC_PWR_MODE values
     */
    typedef enum {
        BNO055_ACC_PWR_MODE_NORMAL              = 0,
        BNO055_ACC_PWR_MODE_SUSPEND             = 1,
        BNO055_ACC_PWR_MODE_LOWPOWER1           = 2,
        BNO055_ACC_PWR_MODE_STANDBY             = 3,
        BNO055_ACC_PWR_MODE_LOWPOWER2           = 4,
        BNO055_ACC_PWR_MODE_DEEPSUSPEND         = 5
    } BNO055_ACC_PWR_MODE_T;

    /**
     * BNO055_REG_MAG_CONFIG bits
     */
    typedef enum {
        BNO055_MAG_CONFIG_MAG_ODR0              = 0x01,
        BNO055_MAG_CONFIG_MAG_ODR1              = 0x02,
        BNO055_MAG_CONFIG_MAG_ODR2              = 0x04,
        _BNO055_MAG_CONFIG_MAG_ODR_MASK         = 7,
        _BNO055_MAG_CONFIG_MAG_ODR_SHIFT        = 0,

        BNO055_MAG_CONFIG_MAG_OPR_MODE0         = 0x08,
        BNO055_MAG_CONFIG_MAG_OPR_MODE1         = 0x10,
        _BNO055_MAG_CONFIG_MAG_OPR_MODE_MASK    = 3,
        _BNO055_MAG_CONFIG_MAG_OPR_MODE_SHIFT   = 3,

        BNO055_MAG_CONFIG_MAG_POWER_MODE0       = 0x20,
        BNO055_MAG_CONFIG_MAG_POWER_MODE1       = 0x40,
        _BNO055_MAG_CONFIG_MAG_POWER_MODE_MASK  = 3,
        _BNO055_MAG_CONFIG_MAG_POWER_MODE_SHIFT = 5
        // 0x80 reserved
    } BNO055_MAG_CONFIG_BITS_T;

    /**
     * MAG_ODR values
     */
    typedef enum {
        BNO055_MAG_ODR_2                        = 0, // 2Hz
        BNO055_MAG_ODR_6                        = 1,
        BNO055_MAG_ODR_8                        = 2,
        BNO055_MAG_ODR_10                       = 3,
        BNO055_MAG_ODR_15                       = 4,
        BNO055_MAG_ODR_20                       = 5,
        BNO055_MAG_ODR_25                       = 6,
        BNO055_MAG_ODR_30                       = 7
    } BNO055_MAG_ODR_T;

    /**
     * BNO055_MAG_OPR values
     */
    typedef enum {
        BNO055_MAG_OPR_LOW                      = 0, // low power
        BNO055_MAG_OPR_REGULAR                  = 1,
        BNO055_MAG_OPR_ENHANCED_REGULAR         = 2,
        BNO055_MAG_OPR_HIGH_ACCURACY            = 3
    } BNO055_MAG_OPR_T;

    /**
     * BNO055_MAG_POWER values
     */
    typedef enum {
        BNO055_MAG_POWER_NORMAL                 = 0,
        BNO055_MAG_POWER_SLEEP                  = 1,
        BNO055_MAG_POWER_SUSPEND                = 2,
        BNO055_MAG_POWER_FORCE_MODE             = 3
    } BNO055_MAG_POWER_T;

    /**
     * BNO055_REG_GYR_CONFIG0 bits
     */
    typedef enum {
        BNO055_GYR_CONFIG0_GYR_RANGE0           = 0x01,
        BNO055_GYR_CONFIG0_GYR_RANGE1           = 0x02,
        BNO055_GYR_CONFIG0_GYR_RANGE2           = 0x04,
        _BNO055_GYR_CONFIG0_GYR_RANGE_MASK      = 7,
        _BNO055_GYR_CONFIG0_GYR_RANGE_SHIFT     = 0,

        BNO055_GYR_CONFIG0_GYR_BW0              = 0x08,
        BNO055_GYR_CONFIG0_GYR_BW1              = 0x10,
        BNO055_GYR_CONFIG0_GYR_BW2              = 0x20,
        _BNO055_GYR_CONFIG0_GYR_BW_MASK         = 7,
        _BNO055_GYR_CONFIG0_GYR_BW_SHIFT        = 3
        // 0x40-0x80 reserved
    } BNO055_GYR_CONFIG0_BITS_T;

    /**
     * BNO055_GYR_RANGE values
     */
    typedef enum {
        BNO055_GYR_RANGE_2000                   = 0, // degrees/sec
        BNO055_GYR_RANGE_1000                   = 1,
        BNO055_GYR_RANGE_500                    = 2,
        BNO055_GYR_RANGE_250                    = 3,
        BNO055_GYR_RANGE_125                    = 4
    } BNO055_GYR_RANGE_T;

    /**
     * BNO055_GYR_BW values
     */
    typedef enum {
        BNO055_GYR_BW_523                       = 0, // Hz
        BNO055_GYR_BW_230                       = 1,
        BNO055_GYR_BW_116                       = 2,
        BNO055_GYR_BW_47                        = 3,
        BNO055_GYR_BW_23                        = 4,
        BNO055_GYR_BW_12                        = 5,
        BNO055_GYR_BW_64                        = 6,
        BNO055_GYR_BW_32                        = 7
    } BNO055_GYR_BW_T;

    /**
     * BNO055_REG_GYR_CONFIG1 bits
     */
    typedef enum {
        BNO055_GYR_CONFIG1_GYR_POWER_MODE0      = 0x01,
        BNO055_GYR_CONFIG1_GYR_POWER_MODE1      = 0x02,
        BNO055_GYR_CONFIG1_GYR_POWER_MODE2      = 0x04,
        _BNO055_GYR_CONFIG1_GYR_POWER_MODE_MASK = 7,
        _BNO055_GYR_CONFIG1_GYR_POWER_MODE_SHIFT = 0
        // 0x08-0x80 reserved
    } BNO055_GYR_CONFIG1_BITS_T;

    /**
     * BNO055_GYR_POWER_MODE values
     */
    typedef enum {
        BNO055_GYR_POWER_MODE_NORMAL            = 0,
        BNO055_GYR_POWER_MODE_FAST_POWERUP      = 1,
        BNO055_GYR_POWER_MODE_DEEP_SUSPEND      = 2,
        BNO055_GYR_POWER_MODE_SUSPEND           = 3,
        BNO055_GYR_POWER_MODE_ADVANCED_POWERSAVE= 4
    } BNO055_GYR_POWER_MODE_T;

    /**
     * BNO055_REG_ACC_SLEEP_CONFIG bits
     */
    typedef enum {
        BNO055_ACC_SLEEP_CONFIG_SLP_MODE        = 0x01, // 0=event,
                                                        // 1=equidistant
                                                        // sample

        BNO055_ACC_SLEEP_CONFIG_ACC_SLP_DUR0    = 0x02,
        BNO055_ACC_SLEEP_CONFIG_ACC_SLP_DUR1    = 0x04,
        BNO055_ACC_SLEEP_CONFIG_ACC_SLP_DUR2    = 0x08,
        BNO055_ACC_SLEEP_CONFIG_ACC_SLP_DUR3    = 0x10,
        _BNO055_ACC_SLEEP_CONFIG_ACC_SLP_DUR_MASK = 15,
        _BNO055_ACC_SLEEP_CONFIG_ACC_SLP_DUR_SHIFT = 1
        // 0x20-0x80 reserved
    } BNO055_ACC_SLEEP_CONFIG_BITS_T;

    /**
     * BNO055_ACC_SLP_DUR values
     */
    typedef enum {
        BNO055_ACC_SLP_DUR_0_5                  = 0, // 0.5ms
        // same for 1-5

        BNO055_ACC_SLP_DUR_1                    = 6, // 1ms
        BNO055_ACC_SLP_DUR_2                    = 7,
        BNO055_ACC_SLP_DUR_4                    = 8,
        BNO055_ACC_SLP_DUR_6                    = 9,
        BNO055_ACC_SLP_DUR_10                   = 10,
        BNO055_ACC_SLP_DUR_25                   = 11,
        BNO055_ACC_SLP_DUR_50                   = 12,
        BNO055_ACC_SLP_DUR_100                  = 13,
        BNO055_ACC_SLP_DUR_500                  = 14
        // 15 = 1ms
    } BNO055_ACC_SLP_DUR_T;

    /**
     * BNO055_REG_GYR_SLEEP_CONFIG bits
     */
    typedef enum {
        BNO055_GYR_SLEEP_CONFIG_GYR_SLEEP_DUR0  = 0x01,
        BNO055_GYR_SLEEP_CONFIG_GYR_SLEEP_DUR1  = 0x02,
        BNO055_GYR_SLEEP_CONFIG_GYR_SLEEP_DUR2  = 0x04,
        _BNO055_GYR_SLEEP_CONFIG_GYR_SLEEP_DUR_MASK = 7,
        _BNO055_GYR_SLEEP_CONFIG_GYR_SLEEP_DUR_SHIFT = 0,

        BNO055_GYR_SLEEP_CONFIG_GYR_AUTO_SLP_DUR0 = 0x08,
        BNO055_GYR_SLEEP_CONFIG_GYR_AUTO_SLP_DUR1 = 0x10,
        BNO055_GYR_SLEEP_CONFIG_GYR_AUTO_SLP_DUR2 = 0x20,
        _BNO055_GYR_SLEEP_CONFIG_GYR_AUTO_SLP_DUR_MASK = 7,
        _BNO055_GYR_SLEEP_CONFIG_GYR_AUTO_SLP_DUR_SHIFT = 3
        // 0x40-0x80 reserved
    } BNO055_GYR_SLEEP_CONFIG_BITS_T;

    /**
     * BNO055_GYR_SLEEP_DUR values
     */
    typedef enum {
        BNO055_GYR_SLEEP_DUR_2                  = 0, // 2ms
        BNO055_GYR_SLEEP_DUR_4                  = 1,
        BNO055_GYR_SLEEP_DUR_5                  = 2,
        BNO055_GYR_SLEEP_DUR_8                  = 3,
        BNO055_GYR_SLEEP_DUR_10                 = 4,
        BNO055_GYR_SLEEP_DUR_15                 = 5,
        BNO055_GYR_SLEEP_DUR_18                 = 6,
        BNO055_GYR_SLEEP_DUR_20                 = 7
    } BNO055_GYR_SLEEP_DUR_T;

    /**
     * BNO055_GYR_AUTO_SLP_DUR values
     */
    typedef enum {
        // 0 = illegal
        BNO055_GYR_AUTO_SLP_DUR_4               = 1, // ms
        BNO055_GYR_AUTO_SLP_DUR_5               = 2,
        BNO055_GYR_AUTO_SLP_DUR_8               = 3,
        BNO055_GYR_AUTO_SLP_DUR_10              = 4,
        BNO055_GYR_AUTO_SLP_DUR_15              = 5,
        BNO055_GYR_AUTO_SLP_DUR_20              = 6,
        BNO055_GYR_AUTO_SLP_DUR_40              = 7
    } BNO055_GYR_AUTO_SLP_DUR_T;

    /**
     * BNO055_REG_INT_MSK and BNO055_REG_INT_EN bits
     */
    typedef enum {
        // 0x00-0x02 reserved
        BNO055_INT_GYRO_AM                      = 0x04, // gyro any-motion
        BNO055_INT_GYRO_HIGH_RATE               = 0x08,
        // 0x10 reserved
        BNO055_INT_ACC_HIGH_G                   = 0x20,
        BNO055_INT_ACC_AM                       = 0x40, // acc any-motion
        BNO055_INT_ACC_NM                       = 0x80, // acc no-motion
    } BNO055_INT_BITS_T;

    /**
     * BNO055_REG_ACC_INT_SETTINGS bits
     */
    typedef enum {
        BNO055_ACC_INT_SETTINGS_AM_DUR0         = 0x01,
        BNO055_ACC_INT_SETTINGS_AM_DUR1         = 0x02,
        _BNO055_ACC_INT_SETTINGS_AM_DUR_MASK    = 3,
        _BNO055_ACC_INT_SETTINGS_AM_DUR_SHIFT   = 0,

        BNO055_ACC_INT_SETTINGS_AM_NM_X_AXIS    = 0x04,
        BNO055_ACC_INT_SETTINGS_AM_NM_Y_AXIS    = 0x08,
        BNO055_ACC_INT_SETTINGS_AM_NM_Z_AXIS    = 0x10,

        BNO055_ACC_INT_SETTINGS_HG_X_AXIS       = 0x20,
        BNO055_ACC_INT_SETTINGS_HG_Y_AXIS       = 0x40,
        BNO055_ACC_INT_SETTINGS_HG_Z_AXIS       = 0x80
    } BNO055_ACC_INT_SETTINGS_BITS_T;

    /**
     * BNO055_REG_ACC_NM_SET bits
     */
    typedef enum {
        BNO055_ACC_NM_SET_SM_NM                 = 0x01, // 0=slowmotion,
                                                        // 1=nomotion

        BNO055_ACC_NM_SET_SM_NM_DUR0            = 0x02,
        BNO055_ACC_NM_SET_SM_NM_DUR1            = 0x04,
        BNO055_ACC_NM_SET_SM_NM_DUR2            = 0x08,
        BNO055_ACC_NM_SET_SM_NM_DUR3            = 0x10,
        BNO055_ACC_NM_SET_SM_NM_DUR4            = 0x20,
        BNO055_ACC_NM_SET_SM_NM_DUR5            = 0x40,
        _BNO055_ACC_NM_SET_SM_NM_DUR_MASK       = 63,
        _BNO055_ACC_NM_SET_SM_NM_DUR_SHIFT      = 1
        // 0x80 reserved
    } BNO055_ACC_NM_SET_BITS_T;

    /**
     * BNO055_REG_GYR_INT_SETTING bits
     */
    typedef enum {
        BNO055_GYR_INT_SETTING_AM_X_AXIS        = 0x01,
        BNO055_GYR_INT_SETTING_AM_Y_AXIS        = 0x02,
        BNO055_GYR_INT_SETTING_AM_Z_AXIS        = 0x04,

        BNO055_GYR_INT_SETTING_HR_X_AXIS        = 0x08,
        BNO055_GYR_INT_SETTING_HR_Y_AXIS        = 0x10,
        BNO055_GYR_INT_SETTING_HR_Z_AXIS        = 0x20,

        BNO055_GYR_INT_SETTING_AM_FILT          = 0x40,
        BNO055_GYR_INT_SETTING_HR_FILT          = 0x80
    } BNO055_GYR_INT_SETTING_BITS_T;

    /**
     * BNO055_REG_GYR_HR_X_SET, BNO055_REG_GYR_HR_Y_SET, and
     * BNO055_REG_GYR_HR_Z_SET bits
     */
    typedef enum {
        BNO055_GYR_HR_XYZ_SET_HR_THRESH0        = 0x01,
        BNO055_GYR_HR_XYZ_SET_HR_THRESH1        = 0x02,
        BNO055_GYR_HR_XYZ_SET_HR_THRESH2        = 0x04,
        BNO055_GYR_HR_XYZ_SET_HR_THRESH3        = 0x08,
        BNO055_GYR_HR_XYZ_SET_HR_THRESH4        = 0x10,
        _BNO055_GYR_HR_XYZ_SET_HR_THRESH_MASK   = 31,
        _BNO055_GYR_HR_XYZ_SET_HR_THRESH_SHIFT  = 0,

        BNO055_GYR_HR_XYZ_SET_HR_THRESH_HYST0   = 0x20,
        BNO055_GYR_HR_XYZ_SET_HR_THRESH_HYST1   = 0x40,
        _BNO055_GYR_HR_XYZ_SET_HR_THRESH_HYST_MASK = 3,
        _BNO055_GYR_HR_XYZ_SET_HR_THRESH_HYST_SHIFT = 5
    } BNO055_GYR_HR_XYZ_SET_BITS_T;

    /**
     * BNO055_REG_GYR_AM_SET bits
     */
    typedef enum {
        BNO055_GYR_AM_SET_SLOPE_SAMPLES0        = 0x01,
        BNO055_GYR_AM_SET_SLOPE_SAMPLES1        = 0x02,
        _BNO055_GYR_AM_SET_SLOPE_SAMPLES_MASK   = 3,
        _BNO055_GYR_AM_SET_SLOPE_SAMPLES_SHIFT  = 0,

        BNO055_GYR_AM_SET_AWAKE_DUR0            = 0x04,
        BNO055_GYR_AM_SET_AWAKE_DUR1            = 0x08,
        _BNO055_GYR_AM_SET_AWAKE_DUR_MASK       = 3,
        _BNO055_GYR_AM_SET_AWAKE_DUR_SHIFT      = 2

        // 0x10-0x80 reserved
    } BNO055_GYR_AM_SET_BITS_T;

    /**
     * BNO055_GYR_AM_SET_SLOPE_SAMPLES values
     */
    typedef enum {
        BNO055_SLOPE_SAMPLES_8                  = 0, // 8 samples
        BNO055_SLOPE_SAMPLES_16                 = 1,
        BNO055_SLOPE_SAMPLES_32                 = 2,
        BNO055_SLOPE_SAMPLES_64                 = 3
    } BNO055_SLOPE_SAMPLES_T;

#ifdef __cplusplus
}
#endif
