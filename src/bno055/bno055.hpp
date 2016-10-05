/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#pragma once

#include <string>
#include <mraa/i2c.hpp>
#include <mraa/gpio.hpp>

#define BNO055_I2C_BUS 0
#define BNO055_DEFAULT_ADDR 0x28

namespace upm {

  /**
   * @brief BNO055 Absolute Orientation 9DOF Fusion Hub
   * @defgroup bno055 libupm-bno055
   * @ingroup i2c gpio accelerometer compass
   */

  /**
   * @library bno055
   * @sensor bno055
   * @comname BNO055 Absolute Orientation 9DOF Fusion Hub
   * @type accelerometer compass
   * @man adafruit
   * @con i2c gpio
   * @web https://www.adafruit.com/products/2472
   *
   * @brief API for the BNO055 Absolute Orientation 9DOF Fusion Hub
   *
   * The BNO055 is a System in Package (SiP), integrating a triaxial
   * 14-bit accelerometer, a triaxial 16-bit gyroscope with a range of
   * +/- 2000 degrees per second, a triaxial geomagnetic sensor and a
   * 32-bit cortex M0+ microcontroller running Bosch Sensortec sensor
   * fusion software, in a single package.
   *
   * This sensor handles the hard problem of combining various sensor
   * information into a reliable measurement of sensor orientation
   * (refered to as 'sensor fusion').  The onboard MCU runs this
   * software and can provide fusion output in the form of Euler
   * Angles, Quaternions, Linear Acceleration, and Gravity Vectors in
   * 3 axes.
   *
   * The focus on this driver has been on supporting the fusion
   * components.  Less support is available for use of this device as
   * a generic accelerometer, gyroscope and magnetometer, however
   * enough infrastructure is available to add any missing
   * functionality.
   *
   * This device requires calibration in order to operate accurately.
   * Methods are provided to retrieve calibration data (once
   * calibrated) to be stored somewhere else, like in a file.  A
   * method is provided to load this data as well.  Calibration data
   * is lost on a power cycle.  See one of the examples for a
   * description of how to calibrate the device, but in essence:
   *
   * There is a calibration status register available
   * (getCalibrationStatus()) that returns the calibration status of
   * the accelerometer (ACC), magnetometer (MAG), gyroscope (GYR), and
   * overall system (SYS).  Each of these values range from 0
   * (uncalibrated) to 3 (fully calibrated).  Calibration involves
   * certain motions to get all 4 values at 3.  The motions are as
   * follows (though see the datasheet for more information):
   *
   * GYR: Simply let the sensor sit flat for a few seconds.
   *
   * ACC: Move the sensor in various positions.  Start flat, then
   * rotate slowly by 45 degrees, hold for a few seconds, then
   * continue rotating another 45 degrees and hold, etc.  6 or more
   * movements of this type may be required.  You can move through any
   * axis you desire, but make sure that the device is lying at least
   * once perpendicular to the x, y, and z axis.
   *
   * MAG: Move slowly in a figure 8 pattern in the air, until the
   * calibration values reaches 3.
   *
   * SYS: This will usually reach 3 when the other items have also
   * reached 3.  If not, continue slowly moving the device though
   * various axes until it does.
   *
   * @snippet bno055.cxx Interesting
   */

  class BNO055 {
  public:
    // The chip ID, for verification in the ctor.
    const uint8_t BNO055_CHIPID = 0xa0;

    // number of bytes of stored calibration data
    const int calibrationDataNumBytes = 22;

    // NOTE: Reserved registers should not be written into.  Reading
    // from them will return indeterminate values.
    //
    // The register map is divided into two pages - page 1 contains
    // sensor specific configuration registers, and page 0 contains all
    // other configuration data and sensor output registers.

    /**
     * BNO055 registers
     */
    typedef enum : uint8_t {
      // The first register listed here is the page ID register.  It
      // is the same on both pages, and selects or indicates the
      // currently active register page.

      REG_PAGE_ID                      = 0x07,

      // Page 0
      REG_CHIP_ID                      = 0x00,
      REG_ACC_ID                       = 0x01, // accel id
      REG_MAG_ID                       = 0x02, // mag id
      REG_GYR_ID                       = 0x03, // gyro id
      REG_SW_REV_ID_LSB                = 0x04,
      REG_SW_REV_ID_MSB                = 0x05,
      REG_BL_REV_ID                    = 0x06, // bootloader rev

      // REG_PAGE_ID = 0x07

      REG_ACC_DATA_X_LSB               = 0x08,
      REG_ACC_DATA_X_MSB               = 0x09,
      REG_ACC_DATA_Y_LSB               = 0x0a,
      REG_ACC_DATA_Y_MSB               = 0x0b,
      REG_ACC_DATA_Z_LSB               = 0x0c,
      REG_ACC_DATA_Z_MSB               = 0x0d,

      REG_MAG_DATA_X_LSB               = 0x0e,
      REG_MAG_DATA_X_MSB               = 0x0f,
      REG_MAG_DATA_Y_LSB               = 0x10,
      REG_MAG_DATA_Y_MSB               = 0x11,
      REG_MAG_DATA_Z_LSB               = 0x12,
      REG_MAG_DATA_Z_MSB               = 0x13,

      REG_GYR_DATA_X_LSB               = 0x14,
      REG_GYR_DATA_X_MSB               = 0x15,
      REG_GYR_DATA_Y_LSB               = 0x16,
      REG_GYR_DATA_Y_MSB               = 0x17,
      REG_GYR_DATA_Z_LSB               = 0x18,
      REG_GYR_DATA_Z_MSB               = 0x19,

      // euler angles
      REG_EUL_HEADING_LSB              = 0x1a,
      REG_EUL_HEADING_MSB              = 0x1b,
      REG_EUL_ROLL_LSB                 = 0x1c,
      REG_EUL_ROLL_MSB                 = 0x1d,
      REG_EUL_PITCH_LSB                = 0x1e,
      REG_EUL_PITCH_MSB                = 0x1f,

      // Quaternions
      REG_QUA_DATA_W_LSB               = 0x20,
      REG_QUA_DATA_W_MSB               = 0x21,
      REG_QUA_DATA_X_LSB               = 0x22,
      REG_QUA_DATA_X_MSB               = 0x23,
      REG_QUA_DATA_Y_LSB               = 0x24,
      REG_QUA_DATA_Y_MSB               = 0x25,
      REG_QUA_DATA_Z_LSB               = 0x26,
      REG_QUA_DATA_Z_MSB               = 0x27,

      // linear accel data
      REG_LIA_DATA_X_LSB               = 0x28,
      REG_LIA_DATA_X_MSB               = 0x29,
      REG_LIA_DATA_Y_LSB               = 0x2a,
      REG_LIA_DATA_Y_MSB               = 0x2b,
      REG_LIA_DATA_Z_LSB               = 0x2c,
      REG_LIA_DATA_Z_MSB               = 0x2d,

      // gravity vector
      REG_GRV_DATA_X_LSB               = 0x2e,
      REG_GRV_DATA_X_MSB               = 0x2f,
      REG_GRV_DATA_Y_LSB               = 0x30,
      REG_GRV_DATA_Y_MSB               = 0x31,
      REG_GRV_DATA_Z_LSB               = 0x32,
      REG_GRV_DATA_Z_MSB               = 0x33,

      REG_TEMPERATURE                  = 0x34,

      REG_CALIB_STAT                   = 0x35, // calibration status
      REG_ST_RESULT                    = 0x36, // selftest result

      REG_INT_STA                      = 0x37, // interrupt status

      REG_SYS_CLK_STATUS               = 0x38,

      REG_SYS_STATUS                   = 0x39,
      REG_SYS_ERROR                    = 0x3a,

      REG_UNIT_SEL                     = 0x3b,

      // 0x3c reserved

      REG_OPER_MODE                    = 0x3d, // operating mode
      REG_POWER_MODE                   = 0x3e,

      REG_SYS_TRIGGER                  = 0x3f,
      REG_TEMP_SOURCE                  = 0x40, // temperature src

      REG_AXIS_MAP_CONFIG              = 0x41,
      REG_AXIS_MAP_SIGN                = 0x42,

      // 0x43-0x54 reserved

      // stored configuration data
      REG_ACC_OFFSET_X_LSB             = 0x55,
      REG_ACC_OFFSET_X_MSB             = 0x56,
      REG_ACC_OFFSET_Y_LSB             = 0x57,
      REG_ACC_OFFSET_Y_MSB             = 0x58,
      REG_ACC_OFFSET_Z_LSB             = 0x59,
      REG_ACC_OFFSET_Z_MSB             = 0x5a,

      REG_MAG_OFFSET_X_LSB             = 0x5b,
      REG_MAG_OFFSET_X_MSB             = 0x5c,
      REG_MAG_OFFSET_Y_LSB             = 0x5d,
      REG_MAG_OFFSET_Y_MSB             = 0x5e,
      REG_MAG_OFFSET_Z_LSB             = 0x5f,
      REG_MAG_OFFSET_Z_MSB             = 0x60,

      REG_GYR_OFFSET_X_LSB             = 0x61,
      REG_GYR_OFFSET_X_MSB             = 0x62,
      REG_GYR_OFFSET_Y_LSB             = 0x63,
      REG_GYR_OFFSET_Y_MSB             = 0x64,
      REG_GYR_OFFSET_Z_LSB             = 0x65,
      REG_GYR_OFFSET_Z_MSB             = 0x66,

      REG_ACC_RADIUS_LSB               = 0x67,
      REG_ACC_RADIUS_MSB               = 0x68,

      REG_MAG_RADIUS_LSB               = 0x69,
      REG_MAG_RADIUS_MSB               = 0x6a,

      // 0x6b-0x7f reserved
      // end of page 0

      // Page 1

      // 0x00-0x06 reserved
      // 0x07 - page id

      REG_ACC_CONFIG                   = 0x08,
      REG_MAG_CONFIG                   = 0x09,
      REG_GYR_CONFIG0                  = 0x0a,
      REG_GYR_CONFIG1                  = 0x0b,
      REG_ACC_SLEEP_CONFIG             = 0x0c,
      REG_GYR_SLEEP_CONFIG             = 0x0d,

      // 0x0e reserved
      REG_INT_MSK                      = 0x0f,
      REG_INT_EN                       = 0x10,

      REG_ACC_AM_THRES                 = 0x11,
      REG_ACC_INT_SETTINGS             = 0x12,
      REG_ACC_HG_DURATION              = 0x13,
      REG_ACC_HG_THRES                 = 0x14,
      REG_ACC_NM_THRES                 = 0x15,
      REG_ACC_NM_SET                   = 0x16,

      REG_GYR_INT_SETTING              = 0x17,
      REG_GYR_HR_X_SET                 = 0x18,
      REG_GYR_DUR_X                    = 0x19,
      REG_GYR_HR_Y_SET                 = 0x1a,
      REG_GYR_DUR_Y                    = 0x1b,
      REG_GYR_HR_Z_SET                 = 0x1c,
      REG_GYR_DUR_Z                    = 0x1d,
      REG_GYR_AM_THRES                 = 0x1e,
      REG_GYR_AM_SET                   = 0x1f,

      // 0x20-0x4f reserved

      // 16 byte (0x50-0x5f) unique ID
      REG_BNO_UNIQUE_ID                = 0x50

      // 0x60-0x7f reserved
    } REGS_T;

    // Page 0 register enumerants

    /**
     * REG_CALIB_STAT bits
     */
    typedef enum {
      CALIB_STAT_MAG0                  = 0x01,
      CALIB_STAT_MAG1                  = 0x02,
      _CALIB_STAT_MAG_MASK             = 3,
      _CALIB_STAT_MAG_SHIFT            = 0,

      CALIB_STAT_ACC0                  = 0x04,
      CALIB_STAT_ACC1                  = 0x08,
      _CALIB_STAT_ACC_MASK             = 3,
      _CALIB_STAT_ACC_SHIFT            = 2,

      CALIB_STAT_GYR0                  = 0x10,
      CALIB_STAT_GYR1                  = 0x20,
      _CALIB_STAT_GYR_MASK             = 3,
      _CALIB_STAT_GYR_SHIFT            = 4,

      CALIB_STAT_SYS0                  = 0x40,
      CALIB_STAT_SYS1                  = 0x80,
      _CALIB_STAT_SYS_MASK             = 3,
      _CALIB_STAT_SYS_SHIFT            = 6
    } CALIB_STAT_BITS_T;

    /**
     * REG_ST_RESULT bits
     */
    typedef enum {
      ST_RESULT_ACC                    = 0x01,
      ST_RESULT_MAG                    = 0x02,
      ST_RESULT_GYR                    = 0x04,
      ST_RESULT_MCU                    = 0x08
      // 0x10-0x80 reserved
    } ST_RESULT_BITS_T;

    /**
     * REG_INT_STA bits
     */
    typedef enum {
      // 0x01-0x02 reserved
      INT_STA_GYRO_AM                  = 0x04, // gyro any-motion
      INT_STA_GYR_HIGH_RATE            = 0x08,
      // 0x010 reserved
      INT_STA_ACC_HIGH_G               = 0x20,
      INT_STA_ACC_AM                   = 0x40, // accel any-motion
      INT_STA_ACC_NM                   = 0x80  // accel no-motion
    } INT_STA_BITS_T;

    /**
     * REG_SYS_CLK_STATUS bits
     */
    typedef enum {
      SYS_CLK_STATUS_ST_MAIN_CLK       = 0x01
      // 0x02-0x80 reserved
    } SYS_CLK_STATUS_BITS_T;

    /**
     * REG_SYS_STATUS values
     */
    typedef enum {
      SYS_STATUS_IDLE                   = 0,
      SYS_STATUS_SYS_ERR                = 1,
      SYS_STATUS_INIT_PERIPHERALS       = 2,
      SYS_STATUS_SYSTEM_INIT            = 3,
      SYS_STATUS_EXECUTING_SELFTEST     = 4,
      SYS_STATUS_FUSION_RUNNING         = 5,
      SYS_STATUS_NO_FUSION_RUNNING      = 6
    } SYS_STATUS_T;

    /**
     * REG_SYS_ERR values
     */
    typedef enum {
      SYS_ERR_NOERROR                   = 0,
      SYS_ERR_PERIPH_INIT_ERROR         = 1,
      SYS_ERR_SYS_INIT_ERROR            = 2,
      SYS_ERR_SELFTEST_FAIL_ERROR       = 3,
      SYS_ERR_REG_VAL_OUTOFRANGE_ERROR  = 4,
      SYS_ERR_REG_ADDR_OUTOFRANGE_ERROR = 5,
      SYS_ERR_REG_WRITE_ERROR           = 6,
      SYS_ERR_LP_MODE_NOT_AVAIL_ERROR   = 7,
      SYS_ERR_ACC_PWR_MODE_NOT_AVAIL_ERROR = 8,
      SYS_ERR_FUSION_CONFIG_ERROR       = 9,
      SYS_ERR_SENSOR_CONFIG_ERROR       = 10
    } SYS_ERR_T;


    /**
     * REG_UNIT_SEL bits
     */
    typedef enum {
      UNIT_SEL_ACC_UNIT                = 0x01, // 0=m/s^2, 1=mg
      UNIT_SEL_GYR_UNIT                = 0x02, // 0=dps, 1=rps
      UNIT_SEL_EUL_UNIT                = 0x04, // 0=degrees, 1=radians
      // 0x08 reserved
      UNIT_SEL_TEMP_UNIT               = 0x10, // 0=C, 1=F
      // 0x20-0x40 reserved
      UNIT_SEL_ORI_ANDROID_WINDOWS     = 0x80 // 0=windows orient, 1=android
    } UNIT_SEL_BITS_T;

    /**
     * REG_OPR_MODE bits
     */
    typedef enum {
      OPR_MODE_OPERATION_MODE0         = 0x01,
      OPR_MODE_OPERATION_MODE1         = 0x02,
      OPR_MODE_OPERATION_MODE2         = 0x04,
      OPR_MODE_OPERATION_MODE3         = 0x08,
      _OPR_MODE_OPERATION_MODE_MASK    = 15,
      _OPR_MODE_OPERATION_MODE_SHIFT   = 0
      // 0x10-0x80 reserved
    } OPR_MODE_BITS_T;

    /**
     * OPR_MODE_OPERATION values
     */
    typedef enum {
      OPERATION_MODE_CONFIGMODE        = 0,
      OPERATION_MODE_ACCONLY           = 1,
      OPERATION_MODE_MAGONLY           = 2,
      OPERATION_MODE_GYROONLY          = 3,
      OPERATION_MODE_ACCMAG            = 4,
      OPERATION_MODE_ACCGYRO           = 5,
      OPERATION_MODE_MAGGYRO           = 6,
      OPERATION_MODE_AMG               = 7,
      // fusion modes
      OPERATION_MODE_IMU               = 8,
      OPERATION_MODE_COMPASS           = 9,
      OPERATION_MODE_M4G               = 10,
      OPERATION_MODE_NDOF_FMC_OFF      = 11,
      OPERATION_MODE_NDOF              = 12
    } OPERATION_MODES_T;

    /**
     * REG_PWR_MODE bits
     */
    typedef enum {
      PWR_MODE_POWER_MODE0             = 0x01,
      PWR_MODE_POWER_MODE1             = 0x02,
      _PWR_MODE_POWER_MODE_MASK        = 3,
      _PWR_MODE_POWER_MODE_SHIFT       = 0
      // 0x04-0x80 reserved
    } PWR_MODE_BITS_T;

    /**
     * POWER_MODE values
     */
    typedef enum {
      POWER_MODE_NORMAL                = 0,
      POWER_MODE_LOW                   = 1,
      POWER_MODE_SUSPEND               = 2
    } POWER_MODES_T;

    /**
     * REG_SYS_TRIGGER bits
     */
    typedef enum {
      SYS_TRIGGER_SELF_TEST            = 0x01,
      // 0x02-0x10 reserved
      SYS_TRIGGER_RST_SYS              = 0x20,
      SYS_TRIGGER_RST_INT              = 0x40,
      SYS_TRIGGER_CLK_SEL              = 0x80
    } SYS_TRIGGER_BITS_T;

    /**
     * REG_TEMP_SOURCE bits
     */
    typedef enum {
      TEMP_SOURCE_TEMP_SOURCE0         = 0x01,
      TEMP_SOURCE_TEMP_SOURCE1         = 0x02,
      _TEMP_SOURCE_TEMP_SOURCE_MASK    = 3,
      _TEMP_SOURCE_TEMP_SOURCE_SHIFT   = 0
      // 0x04-0x80 reserved
    } TEMP_SOURCE_BITS_T;

    /**
     * TEMP_SOURCE values
     */
    typedef enum {
      TEMP_SOURCE_ACC                  = 0,
      TEMP_SOURCE_GYR                  = 1
    } TEMP_SOURCES_T;

    /**
     * REG_AXIS_MAP_CONFIG bits
     */
    typedef enum {
      AXIS_MAP_CONFIG_REMAPPED_X_VAL0  = 0x01,
      AXIS_MAP_CONFIG_REMAPPED_X_VAL1  = 0x02,
      _AXIS_MAP_CONFIG_REMAPPED_X_VAL_MASK = 3,
      _AXIS_MAP_CONFIG_REMAPPED_X_VAL_SHIFT = 0,

      AXIS_MAP_CONFIG_REMAPPED_Y_VAL0  = 0x04,
      AXIS_MAP_CONFIG_REMAPPED_Y_VAL1  = 0x08,
      _AXIS_MAP_CONFIG_REMAPPED_Y_VAL_MASK = 3,
      _AXIS_MAP_CONFIG_REMAPPED_Y_VAL_SHIFT = 2,

      AXIS_MAP_CONFIG_REMAPPED_Z_VAL0  = 0x10,
      AXIS_MAP_CONFIG_REMAPPED_Z_VAL1  = 0x20,
      _AXIS_MAP_CONFIG_REMAPPED_Z_VAL_MASK = 3,
      _AXIS_MAP_CONFIG_REMAPPED_Z_VAL_SHIFT = 4
      // 0x40-0x80 reserved
    } AXIS_MAP_CONFIG_BITS_T;

    /**
     * REMAPPED_AXIS values, applied to X, Y, and Z axes
     * (REG_AXIS_MAP_CONFIG)
     */
    typedef enum {
      REMAPPED_AXIS_X                  = 0,
      REMAPPED_AXIS_Y                  = 1,
      REMAPPED_AXIS_Z                  = 2
    } REMAPPED_AXIS_T;


    /**
     * REG_AXIS_MAP_SIGN bits
     */
    typedef enum {
      AXIS_MAP_SIGN_REMAPPED_Z_SIGN    = 0x01,
      AXIS_MAP_SIGN_REMAPPED_Y_SIGN    = 0x02,
      AXIS_MAP_SIGN_REMAPPED_X_SIGN    = 0x04
      // 0x08-0x80 reserved
    } AXIS_MAP_SIGN_BITS_T;

    // Page 1 register enumerants

    /**
     * REG_ACC_CONFIG bits
     */
    typedef enum {
      ACC_CONFIG_ACC_RANGE0            = 0x01,
      ACC_CONFIG_ACC_RANGE1            = 0x02,
      _ACC_CONFIG_ACC_RANGE_MASK       = 3,
      _ACC_CONFIG_ACC_RANGE_SHIFT      = 0,

      ACC_CONFIG_ACC_BW0               = 0x04,
      ACC_CONFIG_ACC_BW1               = 0x08,
      ACC_CONFIG_ACC_BW2               = 0x10,
      _ACC_CONFIG_ACC_BW_MASK          = 7,
      _ACC_CONFIG_ACC_BW_SHIFT         = 2,

      ACC_CONFIG_ACC_PWR_MODE0         = 0x20,
      ACC_CONFIG_ACC_PWR_MODE1         = 0x40,
      ACC_CONFIG_ACC_PWR_MODE2         = 0x80,
      _ACC_CONFIG_ACC_PWR_MODE_MASK    = 7,
      _ACC_CONFIG_ACC_PWR_MODE_SHIFT   = 5
    } ACC_CONFIG_BITS_T;

    /**
     * ACC_CONFIG_ACC_RANGE values
     */
    typedef enum {
      ACC_RANGE_2G                     = 0,
      ACC_RANGE_4G                     = 1,
      ACC_RANGE_8G                     = 2,
      ACC_RANGE_16G                    = 3
    } ACC_RANGE_T;

    /**
     * ACC_CONFIG_ACC_BW values
     */
    typedef enum {
      ACC_BW_7_81                      = 0, // 7.81 Hz
      ACC_BW_15_53                     = 1,
      ACC_BW_31_25                     = 2,
      ACC_BW_62_5                      = 3,
      ACC_BW_125                       = 4, // 125 Hz
      ACC_BW_250                       = 5,
      ACC_BW_500                       = 6,
      ACC_BW_1000                      = 7
    } ACC_BW_T;

    /**
     * ACC_PWR_MODE values
     */
    typedef enum {
      ACC_PWR_MODE_NORMAL              = 0,
      ACC_PWR_MODE_SUSPEND             = 1,
      ACC_PWR_MODE_LOWPOWER1           = 2,
      ACC_PWR_MODE_STANDBY             = 3,
      ACC_PWR_MODE_LOWPOWER2           = 4,
      ACC_PWR_MODE_DEEPSUSPEND         = 5
    } ACC_PWR_MODE_T;

    /**
     * REG_MAG_CONFIG bits
     */
    typedef enum {
      MAG_CONFIG_MAG_ODR0              = 0x01,
      MAG_CONFIG_MAG_ODR1              = 0x02,
      MAG_CONFIG_MAG_ODR2              = 0x04,
      _MAG_CONFIG_MAG_ODR_MASK         = 7,
      _MAG_CONFIG_MAG_ODR_SHIFT        = 0,

      MAG_CONFIG_MAG_OPR_MODE0         = 0x08,
      MAG_CONFIG_MAG_OPR_MODE1         = 0x10,
      _MAG_CONFIG_MAG_OPR_MODE_MASK    = 3,
      _MAG_CONFIG_MAG_OPR_MODE_SHIFT   = 3,

      MAG_CONFIG_MAG_POWER_MODE0       = 0x20,
      MAG_CONFIG_MAG_POWER_MODE1       = 0x40,
      _MAG_CONFIG_MAG_POWER_MODE_MASK  = 3,
      _MAG_CONFIG_MAG_POWER_MODE_SHIFT = 5
      // 0x80 reserved
    } MAG_CONFIG_BITS_T;

    /**
     * MAG_ODR values
     */
    typedef enum {
      MAG_ODR_2                        = 0, // 2Hz
      MAG_ODR_6                        = 1,
      MAG_ODR_8                        = 2,
      MAG_ODR_10                       = 3,
      MAG_ODR_15                       = 4,
      MAG_ODR_20                       = 5,
      MAG_ODR_25                       = 6,
      MAG_ODR_30                       = 7
    } MAG_ODR_T;

    /**
     * MAG_OPR values
     */
    typedef enum {
      MAG_OPR_LOW                      = 0, // low power
      MAG_OPR_REGULAR                  = 1,
      MAG_OPR_ENHANCED_REGULAR         = 2,
      MAG_OPR_HIGH_ACCURACY            = 3
    } MAG_OPR_T;

    /**
     * MAG_POWER values
     */
    typedef enum {
      MAG_POWER_NORMAL                 = 0,
      MAG_POWER_SLEEP                  = 1,
      MAG_POWER_SUSPEND                = 2,
      MAG_POWER_FORCE_MODE             = 3
    } MAG_POWER_T;

    /**
     * REG_GYR_CONFIG0 bits
     */
    typedef enum {
      GYR_CONFIG0_GYR_RANGE0           = 0x01,
      GYR_CONFIG0_GYR_RANGE1           = 0x02,
      GYR_CONFIG0_GYR_RANGE2           = 0x04,
      _GYR_CONFIG0_GYR_RANGE_MASK      = 7,
      _GYR_CONFIG0_GYR_RANGE_SHIFT     = 0,

      GYR_CONFIG0_GYR_BW0              = 0x08,
      GYR_CONFIG0_GYR_BW1              = 0x10,
      GYR_CONFIG0_GYR_BW2              = 0x20,
      _GYR_CONFIG0_GYR_BW_MASK         = 7,
      _GYR_CONFIG0_GYR_BW_SHIFT        = 3
      // 0x40-0x80 reserved
    } GYR_CONFIG0_BITS_T;

    /**
     * GYR_RANGE values
     */
    typedef enum {
      GYR_RANGE_2000                   = 0, // degrees/sec
      GYR_RANGE_1000                   = 1,
      GYR_RANGE_500                    = 2,
      GYR_RANGE_250                    = 3,
      GYR_RANGE_125                    = 4
    } GYR_RANGE_T;

    /**
     * GYR_BW values
     */
    typedef enum {
      GYR_BW_523                       = 0, // Hz
      GYR_BW_230                       = 1,
      GYR_BW_116                       = 2,
      GYR_BW_47                        = 3,
      GYR_BW_23                        = 4,
      GYR_BW_12                        = 5,
      GYR_BW_64                        = 6,
      GYR_BW_32                        = 7
    } GYR_BW_T;

    /**
     * REG_GYR_CONFIG1 bits
     */
    typedef enum {
      GYR_CONFIG1_GYR_POWER_MODE0      = 0x01,
      GYR_CONFIG1_GYR_POWER_MODE1      = 0x02,
      GYR_CONFIG1_GYR_POWER_MODE2      = 0x04,
      _GYR_CONFIG1_GYR_POWER_MODE_MASK = 7,
      _GYR_CONFIG1_GYR_POWER_MODE_SHIFT = 0
      // 0x08-0x80 reserved
    } GYR_CONFIG1_BITS_T;

    /**
     * GYR_POWER_MODE values
     */
    typedef enum {
      GYR_POWER_MODE_NORMAL            = 0,
      GYR_POWER_MODE_FAST_POWERUP      = 1,
      GYR_POWER_MODE_DEEP_SUSPEND      = 2,
      GYR_POWER_MODE_SUSPEND           = 3,
      GYR_POWER_MODE_ADVANCED_POWERSAVE= 4
    } GYR_POWER_MODE_T;

    /**
     * REG_ACC_SLEEP_CONFIG bits
     */
    typedef enum {
      ACC_SLEEP_CONFIG_SLP_MODE        = 0x01, // 0=event, 1=equidistant sample

      ACC_SLEEP_CONFIG_ACC_SLP_DUR0    = 0x02,
      ACC_SLEEP_CONFIG_ACC_SLP_DUR1    = 0x04,
      ACC_SLEEP_CONFIG_ACC_SLP_DUR2    = 0x08,
      ACC_SLEEP_CONFIG_ACC_SLP_DUR3    = 0x10,
      _ACC_SLEEP_CONFIG_ACC_SLP_DUR_MASK = 15,
      _ACC_SLEEP_CONFIG_ACC_SLP_DUR_SHIFT = 1
      // 0x20-0x80 reserved
    } ACC_SLEEP_CONFIG_BITS_T;

    /**
     * ACC_SLP_DUR values
     */
    typedef enum {
      ACC_SLP_DUR_0_5                  = 0, // 0.5ms
      // same for 1-5

      ACC_SLP_DUR_1                    = 6, // 1ms
      ACC_SLP_DUR_2                    = 7,
      ACC_SLP_DUR_4                    = 8,
      ACC_SLP_DUR_6                    = 9,
      ACC_SLP_DUR_10                   = 10,
      ACC_SLP_DUR_25                   = 11,
      ACC_SLP_DUR_50                   = 12,
      ACC_SLP_DUR_100                  = 13,
      ACC_SLP_DUR_500                  = 14
      // 15 = 1ms
    } ACC_SLP_DUR_T;

    /**
     * REG_GYR_SLEEP_CONFIG bits
     */
    typedef enum {
      GYR_SLEEP_CONFIG_GYR_SLEEP_DUR0  = 0x01,
      GYR_SLEEP_CONFIG_GYR_SLEEP_DUR1  = 0x02,
      GYR_SLEEP_CONFIG_GYR_SLEEP_DUR2  = 0x04,
      _GYR_SLEEP_CONFIG_GYR_SLEEP_DUR_MASK = 7,
      _GYR_SLEEP_CONFIG_GYR_SLEEP_DUR_SHIFT = 0,

      GYR_SLEEP_CONFIG_GYR_AUTO_SLP_DUR0 = 0x08,
      GYR_SLEEP_CONFIG_GYR_AUTO_SLP_DUR1 = 0x10,
      GYR_SLEEP_CONFIG_GYR_AUTO_SLP_DUR2 = 0x20,
      _GYR_SLEEP_CONFIG_GYR_AUTO_SLP_DUR_MASK = 7,
      _GYR_SLEEP_CONFIG_GYR_AUTO_SLP_DUR_SHIFT = 3
      // 0x40-0x80 reserved
    } GYR_SLEEP_CONFIG_BITS_T;

    /**
     * GYR_SLEEP_DUR values
     */
    typedef enum {
      GYR_SLEEP_DUR_2                  = 0, // 2ms
      GYR_SLEEP_DUR_4                  = 1,
      GYR_SLEEP_DUR_5                  = 2,
      GYR_SLEEP_DUR_8                  = 3,
      GYR_SLEEP_DUR_10                 = 4,
      GYR_SLEEP_DUR_15                 = 5,
      GYR_SLEEP_DUR_18                 = 6,
      GYR_SLEEP_DUR_20                 = 7
    } GYR_SLEEP_DUR_T;

    /**
     * GYR_AUTO_SLP_DUR values
     */
    typedef enum {
      // 0 = illegal
      GYR_AUTO_SLP_DUR_4               = 1, // ms
      GYR_AUTO_SLP_DUR_5               = 2,
      GYR_AUTO_SLP_DUR_8               = 3,
      GYR_AUTO_SLP_DUR_10              = 4,
      GYR_AUTO_SLP_DUR_15              = 5,
      GYR_AUTO_SLP_DUR_20              = 6,
      GYR_AUTO_SLP_DUR_40              = 7
    } GYR_AUTO_SLP_DUR_T;

    /**
     * REG_INT_MSK and REG_INT_EN bits
     */
    typedef enum {
      // 0x00-0x02 reserved
      INT_GYRO_AM                      = 0x04, // gyro any-motion
      INT_GYRO_HIGH_RATE               = 0x08,
      // 0x10 reserved
      INT_ACC_HIGH_G                   = 0x20,
      INT_ACC_AM                       = 0x40, // acc any-motion
      INT_ACC_NM                       = 0x80, // acc no-motion
    } INT_BITS_T;

    /**
     * REG_ACC_INT_SETTINGS bits
     */
    typedef enum {
      ACC_INT_SETTINGS_AM_DUR0         = 0x01,
      ACC_INT_SETTINGS_AM_DUR1         = 0x02,
      _ACC_INT_SETTINGS_AM_DUR_MASK    = 3,
      _ACC_INT_SETTINGS_AM_DUR_SHIFT   = 0,

      ACC_INT_SETTINGS_AM_NM_X_AXIS    = 0x04,
      ACC_INT_SETTINGS_AM_NM_Y_AXIS    = 0x08,
      ACC_INT_SETTINGS_AM_NM_Z_AXIS    = 0x10,

      ACC_INT_SETTINGS_HG_X_AXIS       = 0x20,
      ACC_INT_SETTINGS_HG_Y_AXIS       = 0x40,
      ACC_INT_SETTINGS_HG_Z_AXIS       = 0x80
    } ACC_INT_SETTINGS_BITS_T;

    /**
     * REG_ACC_NM_SET bits
     */
    typedef enum {
      ACC_NM_SET_SM_NM                 = 0x01, // 0=slowmotion, 1=nomotion

      ACC_NM_SET_SM_NM_DUR0            = 0x02,
      ACC_NM_SET_SM_NM_DUR1            = 0x04,
      ACC_NM_SET_SM_NM_DUR2            = 0x08,
      ACC_NM_SET_SM_NM_DUR3            = 0x10,
      ACC_NM_SET_SM_NM_DUR4            = 0x20,
      ACC_NM_SET_SM_NM_DUR5            = 0x40,
      _ACC_NM_SET_SM_NM_DUR_MASK       = 63,
      _ACC_NM_SET_SM_NM_DUR_SHIFT      = 1
      // 0x80 reserved
    } ACC_NM_SET_BITS_T;

    /**
     * REG_GYR_INT_SETTING bits
     */
    typedef enum {
      GYR_INT_SETTING_AM_X_AXIS        = 0x01,
      GYR_INT_SETTING_AM_Y_AXIS        = 0x02,
      GYR_INT_SETTING_AM_Z_AXIS        = 0x04,

      GYR_INT_SETTING_HR_X_AXIS        = 0x08,
      GYR_INT_SETTING_HR_Y_AXIS        = 0x10,
      GYR_INT_SETTING_HR_Z_AXIS        = 0x20,

      GYR_INT_SETTING_AM_FILT          = 0x40,
      GYR_INT_SETTING_HR_FILT          = 0x80
    } GYR_INT_SETTING_BITS_T;

    /**
     * REG_GYR_HR_X_SET, REG_GYR_HR_Y_SET, and REG_GYR_HR_Z_SET bits
     */
    typedef enum {
      GYR_HR_XYZ_SET_HR_THRESH0        = 0x01,
      GYR_HR_XYZ_SET_HR_THRESH1        = 0x02,
      GYR_HR_XYZ_SET_HR_THRESH2        = 0x04,
      GYR_HR_XYZ_SET_HR_THRESH3        = 0x08,
      GYR_HR_XYZ_SET_HR_THRESH4        = 0x10,
      _GYR_HR_XYZ_SET_HR_THRESH_MASK   = 31,
      _GYR_HR_XYZ_SET_HR_THRESH_SHIFT  = 0,

      GYR_HR_XYZ_SET_HR_THRESH_HYST0   = 0x20,
      GYR_HR_XYZ_SET_HR_THRESH_HYST1   = 0x40,
      _GYR_HR_XYZ_SET_HR_THRESH_HYST_MASK = 3,
      _GYR_HR_XYZ_SET_HR_THRESH_HYST_SHIFT = 5
    } GYR_HR_XYZ_SET_BITS_T;

    /**
     * REG_GYR_AM_SET bits
     */
    typedef enum {
      GYR_AM_SET_SLOPE_SAMPLES0        = 0x01,
      GYR_AM_SET_SLOPE_SAMPLES1        = 0x02,
      _GYR_AM_SET_SLOPE_SAMPLES_MASK   = 3,
      _GYR_AM_SET_SLOPE_SAMPLES_SHIFT  = 0,

      GYR_AM_SET_AWAKE_DUR0            = 0x04,
      GYR_AM_SET_AWAKE_DUR1            = 0x08,
      _GYR_AM_SET_AWAKE_DUR_MASK       = 3,
      _GYR_AM_SET_AWAKE_DUR_SHIFT      = 2

      // 0x10-0x80 reserved
    } GYR_AM_SET_BITS_T;

    /**
     * GYR_AM_SET_SLOPE_SAMPLES values
     */
    typedef enum {
      SLOPE_SAMPLES_8                  = 0, // 8 samples
      SLOPE_SAMPLES_16                 = 1,
      SLOPE_SAMPLES_32                 = 2,
      SLOPE_SAMPLES_64                 = 3
    } SLOPE_SAMPLES_T;

    /**
     * BNO055 constructor.
     *
     * By default, the constructor sets the acceleration units to
     * m/s^2, gyro and Euler units to degrees, and temperature to
     * celsius.  It then enters the NDOF fusion mode.
     *
     * In addition, the internal clock is used so that compatibility
     * with other implementations is assured.  If you are using a
     * device with an external clock, call setClockExternal(true) to
     * enable it.
     *
     * @param bus I2C bus to use.
     * @param address The address for this device.
     */
    BNO055(int bus=BNO055_I2C_BUS, uint8_t addr=BNO055_DEFAULT_ADDR);

    /**
     * BNO055 Destructor.
     */
    ~BNO055();

    /**
     * Update the internal stored values from sensor data.
     */
    void update();

    /**
     * Return the chip ID.
     *
     * @return The chip ID (BNO055_CHIPID).
     */
    uint8_t getChipID();

    /**
     * Return the accelerometer chip ID.
     *
     * @return The chip ID.
     */
    uint8_t getACCID();

    /**
     * Return the magnetometer chip ID.
     *
     * @return The chip ID.
     */
    uint8_t getMAGID();

    /**
     * Return the gyroscope chip ID.
     *
     * @return The chip ID.
     */
    uint8_t getGYRID();

    /**
     * Return the fusion firmware revison.
     *
     * @return The firmware revison.
     */
    uint16_t getSWRevID();

    /**
     * Return the bootloader ID.
     *
     * @return The bootloader ID.
     */
    uint8_t getBootLoaderID();

    /**
     * Enable or disables the use of the external clock.  The Adafriut
     * device does contain an external clock which might be more
     * stable.  By default, the internal clock is used.
     *
     * @param extClock true to use external clock, false otherwise.
     */
    void setClockExternal(bool extClock);

    /**
     * Select the temperature source.  This can be the accelerometer
     * or the gyroscope.  By default, the accelerometer temperature is
     * used as the source.
     *
     * @param src One of the TEMP_SOURCES_T values.
     */
    void setTemperatureSource(TEMP_SOURCES_T src);

    /**
     * Select the temperature units.  This can be the Fahrenheit or
     * Celsius.
     *
     * @param celsius true for Celius, false for Fahrenheit.
     */
    void setTemperatureUnits(bool celsius);

    /**
     * Set the operating mode for the device.  This places the device
     * into a config mode, one of 7 non-fusion modes, or one of 5
     * fusion modes.  All stored sensor data is cleared when switching
     * modes.  The device must be in config mode for most
     * configuration operations.  See the datasheet for details.
     *
     * @param mode One of the OPERATION_MODES_T values.
     */
    void setOperationMode(OPERATION_MODES_T mode);

    /**
     * Reboot the sensor.  This is equivalent to a power on reset.
     * All calibration data will be lost, and the device must be
     * recalibrated.
     */
    void resetSystem();

    /**
     * Read the calibration status registers and return them.  The
     * values range from 0 (uncalibrated) to 3 (fully calibrated).
     *
     * @param mag The calibration status of the magnetometer.
     * @param acc The calibration status of the accelerometer.
     * @param mag The calibration status of the gyroscope.
     * @param mag The calibration status of the overall system.
     */
    void getCalibrationStatus(int *mag, int *acc, int *gyr, int *sys);

    /**
     * Read the calibration status registers and return them as an
     * integer array.  The values range from 0 (uncalibrated) to 3
     * (fully calibrated).
     *
     * @return An integer array containing the values in the order:
     * mag, acc, gyr, and sys.
     */
    int *getCalibrationStatus();

    /**
     * Read the calibration status registers and return true or false,
     * indicating whether all of the calibration parameters are fully
     * calibrated.
     *
     * @return true if all 4 calibration parameters are fully
     * calibrated, else false.
     */
    bool isFullyCalibrated();

    /**
     * Read the calibration data and return it as a string.  This data
     * can then be saved for later reuse by writeCalibrationData() to
     * restore calibration data after a reset.
     *
     * @return string representing calibration data.
     */
    std::string readCalibrationData();

    /**
     * Write previously saved calibration data to the calibration
     * registers.
     *
     * @param string representing calibration data, as returned by
     * readCalibrationData().
     */
    void writeCalibrationData(std::string calibData);

    /**
     * Return the current measured temperature.  Note, this is not
     * ambient temperature - this is the temperature of the selected
     * source on the chip.  update() must have been called prior to
     * calling this method.
     *
     * @param fahrenheit true to return data in Fahrenheit, false for
     * Celicus.  Celsius is the default.
     * @return The temperature in degrees Celsius or Fahrenheit.
     */
    float getTemperature(bool fahrenheit=false);

    /**
     * Return current orientation fusion data in the form of Euler
     * Angles.  By default, the returned values are in degrees.
     * update() must have been called prior to calling this method.
     *
     * @param heading Pointer to a floating point value that will have
     * the current heading angle placed into it.
     * @param roll Pointer to a floating point value that will have
     * the current roll angle placed into it.
     * @param pitch Pointer to a floating point value that will have
     * the current pitch angle placed into it.
     */
    void getEulerAngles(float *heading, float *roll, float *pitch);

    /**
     * Return current orientation fusion data in the form of Euler
     * Angles as a floating point array.  By default, the returned
     * values are in degrees.  update() must have been called prior to
     * calling this method.
     *
     * @return A floating point array containing heading, roll, and
     * pitch, in that order.
     */
    float *getEulerAngles();

    /**
     * Return current orientation fusion data in the form of
     * Quaternions.  update() must have been called prior to calling
     * this method.
     *
     * @param w Pointer to a floating point value that will have
     * the current w component placed into it.
     * @param x Pointer to a floating point value that will have
     * the current x component placed into it.
     * @param y Pointer to a floating point value that will have
     * the current y component placed into it.
     * @param z Pointer to a floating point value that will have
     * the current z component placed into it.
     */
    void getQuaternions(float *w, float *x, float *y, float *z);

    /**
     * Return current orientation fusion data in the form of
     * Quaternions, as a floating point array.  update() must have
     * been called prior to calling this method.
     *
     * @return A floating point array containing w, x, y, and z in
     * that order.
     */
    float *getQuaternions();

    /**
     * Return current orientation fusion data in the form of Linear
     * Acceleration.  By default the returned values are in meters
     * per-second squared (m/s^2).  update() must have been called
     * prior to calling this method.
     *
     * @param x Pointer to a floating point value that will have
     * the current x component placed into it.
     * @param y Pointer to a floating point value that will have
     * the current y component placed into it.
     * @param z Pointer to a floating point value that will have
     * the current z component placed into it.
     */
    void getLinearAcceleration(float *x, float *y, float *z);

    /**
     * Return current orientation fusion data in the form of Linear
     * Acceleration, as a floating point array.  update() must have
     * been called prior to calling this method.
     *
     * @return A floating point array containing x, y, and z in
     * that order.
     */
    float *getLinearAcceleration();

    /**
     * Return current orientation fusion data in the form of a Gravity
     * Vector per-axis.  By default the returned values are in meters
     * per-second squared (m/s^2).  update() must have been called
     * prior to calling this method.
     *
     * @param x Pointer to a floating point value that will have
     * the current x component placed into it.
     * @param y Pointer to a floating point value that will have
     * the current y component placed into it.
     * @param z Pointer to a floating point value that will have
     * the current z component placed into it.
     */
    void getGravityVectors(float *x, float *y, float *z);

    /**
     * Return current orientation fusion data in the form of a Gravity
     * Vector per-axis as a floating point array.  update() must have
     * been called prior to calling this method.
     *
     * @return A floating point array containing x, y, and z in
     * that order.
     */
    float *getGravityVectors();

    /**
     * Return uncompensated accelerometer data (non-fusion).  In
     * fusion modes, this data will be of little value.  By default
     * the returned values are in meters per-second squared (m/s^2).
     * update() must have been called prior to calling this method.
     *
     * @param x Pointer to a floating point value that will have
     * the current x component placed into it.
     * @param y Pointer to a floating point value that will have
     * the current y component placed into it.
     * @param z Pointer to a floating point value that will have
     * the current z component placed into it.
     */
    void getAccelerometer(float *x, float *y, float *z);

    /**
     * Return current uncompensated accelerometer (non-fusion) data in
     * the form of a floating point array.  By default the returned
     * values are in meters per-second squared (m/s^2).  update() must
     * have been called prior to calling this method.
     *
     * @return A floating point array containing x, y, and z in
     * that order.
     */
    float *getAccelerometer();

    /**
     * Return uncompensated magnetometer data (non-fusion).  In fusion
     * modes, this data will be of little value.  The returned values
     * are in micro-teslas (uT).  update() must have been called prior
     * to calling this method.
     *
     * @param x Pointer to a floating point value that will have
     * the current x component placed into it.
     * @param y Pointer to a floating point value that will have
     * the current y component placed into it.
     * @param z Pointer to a floating point value that will have
     * the current z component placed into it.
     */
    void getMagnetometer(float *x, float *y, float *z);

    /**
     * Return current uncompensated magnetometer (non-fusion) data in
     * the form of a floating point array.  The returned values are in
     * micro-teslas (uT).  update() must have been called prior to
     * calling this method.
     *
     * @return A floating point array containing x, y, and z in
     * that order.
     */
    float *getMagnetometer();

    /**
     * Return uncompensated gyroscope data (non-fusion).  In fusion
     * modes, this data will be of little value.  By default the
     * returned values are in meters per-second squared (m/s^2).
     * update() must have been called prior to calling this method.
     *
     * @param x Pointer to a floating point value that will have
     * the current x component placed into it.
     * @param y Pointer to a floating point value that will have
     * the current y component placed into it.
     * @param z Pointer to a floating point value that will have
     * the current z component placed into it.
     */
    void getGyroscope(float *x, float *y, float *z);

    /**
     * Return current uncompensated gyroscope (non-fusion) data in the
     * form of a floating point array.  By default the returned values
     * are in meters per-second squared (m/s^2).  update() must have
     * been called prior to calling this method.
     *
     * @return A floating point array containing x, y, and z in
     * that order.
     */
    float *getGyroscope();

    /**
     * Set the bandwidth, range, and power modes of the accelerometer.
     * In fusion modes, these values will be ignored.
     *
     * @param range One of the ACC_RANGE_T values.
     * @param bw One of the ACC_BW_T values.
     * @param pwr One of the ACC_PWR_MODE_T values.
     */
    void setAccelerationConfig(ACC_RANGE_T range, ACC_BW_T bw,
                               ACC_PWR_MODE_T pwr);

    /**
     * Set the output data rate, operating mode and power mode of the
     * magnetometer.  In fusion modes, these values will be ignored.
     *
     * @param odr One of the MAG_ODR_T values.
     * @param opr One of the MAG_OPR_T values.
     * @param pwr One of the MAG_POWER_T values.
     */
    void setMagnetometerConfig(MAG_ODR_T odr, MAG_OPR_T opr,
                               MAG_POWER_T pwr);

    /**
     * Set the range, bandwidth and power modes of the gyroscope.  In
     * fusion modes, these values will be ignored.
     *
     * @param range One of the GYR_RANGE_T values.
     * @param bw One of the GYR_BW_T values.
     * @param pwr One of the GYR_POWER_MODE_T values.
     */
    void setGyroscopeConfig(GYR_RANGE_T range, GYR_BW_T bw,
                            GYR_POWER_MODE_T pwr);

    /**
     * Set the unit of measurement for the accelerometer related
     * sensor values.  The choices are mg (milligrams) or meters
     * per-second squared (m/s^2).  The default is m/s^2.
     *
     * @param mg true for mg, false for m/s^2.
     */
    void setAccelerometerUnits(bool mg=false);

    /**
     * Set the unit of measurement for the gyroscope related sensor
     * values.  The choices are degrees and radians.  The default is
     * degrees.
     *
     * @param radians true for radians, false for degrees.
     */
    void setGyroscopeUnits(bool radians=false);

    /**
     * Set the unit of measurement for the Euler Angle related sensor
     * values.  The choices are degrees and radians.  The default is
     * degrees.
     *
     * @param radians true for radians, false for degrees.
     */
    void setEulerUnits(bool radians=false);

    /**
     * Reset all interrupt status bits and interrupt output.
     */
    void resetInterruptStatus();

    /**
     * Return the interrupt status register.  This is a bitmask of the
     * INT_STA_BITS_T bits.
     *
     * @return a bitmask of INT_STA_BITS_T bits.
     */
    uint8_t getInterruptStatus();

    /**
     * Return the interrupt enables register.  This is a bitmask of the
     * INT_STA_BITS_T bits.
     *
     * @return a bitmask of INT_STA_BITS_T bits currently set in the
     * enable register.
     */
    uint8_t getInterruptEnable();

    /**
     * Set the interrupt enable register. This is composed of a
     * bitmask of the INT_STA_BITS_T bits.
     *
     * @param enables a bitmask of INT_STA_BITS_T bits to enable
     */
    void setInterruptEnable(uint8_t enables);

    /**
     * Return the interrupt mask register.  This is a bitmask of the
     * INT_STA_BITS_T bits.  The interrupt mask is used to mask off
     * enabled interrupts from generating a hardware interrupt.  The
     * interrupt status register can still be used to detect masked
     * interrupts if they are enabled.
     *
     * @return a bitmask of INT_STA_BITS_T bits currently set in the
     * interrupt mask register.
     */
    uint8_t getInterruptMask();

    /**
     * Set the interrupt mask register.  This is a bitmask of the
     * INT_STA_BITS_T bits.  The interrupt mask is used to mask off
     * enabled interrupts from generating a hardware interrupt.  The
     * interrupt status register can still be used to detect masked
     * interrupts if they are enabled.
     *
     * @param a bitmask of INT_STA_BITS_T bits to set in the interrupt
     * mask register.
     */
    void setInterruptMask(uint8_t mask);

    /**
     * Return the value of the system status register.  This method
     * can be used to determine the overall status of the device.
     *
     * @return One of the SYS_STATUS_T values.
     */
    SYS_STATUS_T getSystemStatus();

    /**
     * Return the value of the system error register.  This mathod can
     * be used to determine a variety of system related error
     * conditions.
     *
     * @return One of the SYS_ERR_T values.
     */
    SYS_ERR_T getSystemError();


#if defined(SWIGJAVA) || defined(JAVACALLBACK)
    void installISR(int gpio, mraa::Edge level, jobject runnable);
#else
    /**
     * install an interrupt handler.
     *
     * @param gpio gpio pin to use as interrupt pin
     * @param level the interrupt trigger level (one of mraa::Edge
     * values).  Make sure that you have configured the interrupt pin
     * properly for whatever level you choose.
     * @param isr the interrupt handler, accepting a void * argument
     * @param arg the argument to pass the the interrupt handler
     */
    void installISR(int gpio, mraa::Edge level,
                    void (*isr)(void *), void *arg);
#endif

   /**
     * uninstall a previously installed interrupt handler
     *
     */
    void uninstallISR();

  protected:
    mraa::I2c m_i2c;
    mraa::Gpio *m_gpioIntr;
    uint8_t m_addr;

    // always stored in C
    float m_temperature;

    // uncompensated data

    // mag data
    float m_magX;
    float m_magY;
    float m_magZ;

    // acc data
    float m_accX;
    float m_accY;
    float m_accZ;

    // acc units
    float m_accUnitScale;

    // gyr data
    float m_gyrX;
    float m_gyrY;
    float m_gyrZ;

    // gyr units
    float m_gyrUnitScale;

    // eul (euler angle) data
    float m_eulHeading;
    float m_eulRoll;
    float m_eulPitch;

    // eul units
    float m_eulUnitScale;

    // qua (quaternion) data
    float m_quaW;
    float m_quaX;
    float m_quaY;
    float m_quaZ;

    // lia (linear acceleration) data
    float m_liaX;
    float m_liaY;
    float m_liaZ;

    // grv (gravity vector) data
    float m_grvX;
    float m_grvY;
    float m_grvZ;

    void clearData();
    bool updateFusionData();
    bool updateNonFusionData();
    void setPage(uint8_t page, bool force=false);

    /**
     * Read a register.
     *
     * @param reg The register to read
     * @return The value of the register
     */
    uint8_t readReg(uint8_t reg);

    /**
     * Read contiguous registers into a buffer.
     *
     * @param buffer The buffer to store the results
     * @param len The number of registers to read
     */
    void readRegs(uint8_t reg, uint8_t *buffer, int len);

    /**
     * Write to a register
     *
     * @param reg The register to write to
     * @param val The value to write
     * @return true if successful, false otherwise
     */
    bool writeReg(uint8_t reg, uint8_t val);

    /**
     * Write data to contiguous registers
     *
     * @param reg The starting register to write to
     * @param buffer The buffer containing the data to write
     * @param len The number of bytes to write
     * @return true if successful, false otherwise
     */
    bool writeRegs(uint8_t reg, uint8_t *buffer, int len);

  private:
    int m_currentPage;
    OPERATION_MODES_T m_currentMode;
    bool m_tempIsC;

    // Adding a private function definition for java bindings
#if defined(SWIGJAVA) || defined(JAVACALLBACK)
    void installISR(int gpio, mraa::Edge level,
                    void (*isr)(void *), void *arg);
#endif
  };
}
