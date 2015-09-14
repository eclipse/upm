/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
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
#include <mraa/common.hpp>
#include <mraa/i2c.hpp>

#include <mraa/gpio.hpp>

#if defined(SWIGJAVA) || defined(JAVACALLBACK)
#include "../IsrCallback.h"
#endif

#define LSM9DS0_I2C_BUS 1
#define LSM9DS0_DEFAULT_XM_ADDR 0x1d
#define LSM9DS0_DEFAULT_GYRO_ADDR 0x6b

namespace upm {
  
  /**
   * @brief LSM9DS0 accelerometer library
   * @defgroup lsm9ds0 libupm-lsm9ds0
   * @ingroup i2c gpio accelerometer compass
   */

  /**
   * @library lsm9ds0
   * @sensor lsm9ds0
   * @comname LSM9DS0 3-axis Gyroscope, Accelerometer, and Magnetometer
   * @type accelerometer compass
   * @man sparkfun
   * @con i2c gpio
   * @web https://www.sparkfun.com/products/13033
   *
   * @brief API for the LSM9DS0 3-axis Gyroscope, Accelerometer, 
   * and Magnetometer
   *
   * The LSM9DS0 is a system-in-package featuring a 3D digital linear
   * acceleration sensor, a 3D digital angular rate sensor, and a 3D
   * digital magnetic sensor.
   *
   * The LSM9DS0 has a linear acceleration full scale of
   * 2g/4g/6g/8g/16g, a magnetic field full scale of 2/4/8/12
   * gauss and an angular rate of 245/500/2000 dps.
   *
   * While not all of the functionality of this device is supported
   * initially, methods and register definitions are provided that
   * should allow an end user to implement whatever features are
   * required.
   *
   * This driver was developed on a Sparkfun 9DOF edison block.
   *
   * @snippet lsm9ds0.cxx Interesting
   */

  class LSM9DS0 {
  public:

    // NOTE: reserved registers must not be written into or permanent
    // damage to the device can result.  Reserved bitfields must
    // always be 0.

    // There are two sub-devices within this device - the
    // Accelerometer and Magnetometer (XM) and the Gyroscope (G), each
    // with their own I2C address.

    /**
     * LSM9DS0 Gyroscope (G) registers
     */
    typedef enum {
      // 0x00-0x0e reserved

      REG_WHO_AM_I_G            = 0x0f, // should be 0xd4

      // 0x10-0x1f reserved

      REG_CTRL_REG1_G           = 0x20,
      REG_CTRL_REG2_G           = 0x21,
      REG_CTRL_REG3_G           = 0x22,
      REG_CTRL_REG4_G           = 0x23,
      REG_CTRL_REG5_G           = 0x24,

      REG_REFERENCE_G           = 0x25,

      // 0x26 reserved

      REG_STATUS_REG_G          = 0x27,

      REG_OUT_X_L_G             = 0x28, // gyro output, X axis, LSB 
      REG_OUT_X_H_G             = 0x29, // gyro output, X axis, MSB 
      REG_OUT_Y_L_G             = 0x2a,
      REG_OUT_Y_H_G             = 0x2b,
      REG_OUT_Z_L_G             = 0x2c,
      REG_OUT_Z_H_G             = 0x2d,
      
      REG_FIFO_CTRL_REG_G       = 0x2e,
      REG_FIFO_SRC_REG_G        = 0x2f,

      REG_INT1_CFG_G            = 0x30,
      REG_INT1_SRC_G            = 0x31,

      REG_INT1_TSH_XH_G         = 0x32, // interrupt threshold registers
      REG_INT1_TSH_XL_G         = 0x33,
      REG_INT1_TSH_YH_G         = 0x34,
      REG_INT1_TSH_YL_G         = 0x35,
      REG_INT1_TSH_ZH_G         = 0x36,
      REG_INT1_TSH_ZL_G         = 0x37,

      // See fig 19 & 20 and preceeding description in the datasheet
      // on how to use this register
      REG_INT1_DURATION_G       = 0x38
    } REG_G_T;
    
    /**
     * Gyro CTRL_REG1_G bits
     */
    typedef enum {
      CTRL_REG1_G_YEN           = 0x01, // Y enable, odd ordering...
      CTRL_REG1_G_XEN           = 0x02, 
      CTRL_REG1_G_ZEN           = 0x04, 
      CTRL_REG1_G_PD            = 0x08, // power down (0)

      CTRL_REG1_G_BW0           = 0x10, // bandwidth
      CTRL_REG1_G_BW1           = 0x20,
      _CTRL_REG1_G_BW_MASK      = 3,
      _CTRL_REG1_G_BW_SHIFT     = 4,

      CTRL_REG1_G_DR0           = 0x40, // data rate
      CTRL_REG1_G_DR1           = 0x80,
      _CTRL_REG1_G_DR_MASK      = 3,
      _CTRL_REG1_G_DR_SHIFT     = 6,

      // The following are synthetic register and shift/mask
      // definitions.  Together both BW and DR setup the device for a
      // specific output data rate (ODR) and cutoff frequency. These
      // definitions allow us to use a more informative configuration
      // for these 4 bits, rather than having the user go to the
      // datasheet to figure out what to put for those values in order
      // to get the desired ODR/cutoff.  These are the values we will
      // use in this driver.

      CTRL_REG1_G_ODR0          = 0x10, // BW0
      CTRL_REG1_G_ODR1          = 0x20, // BW1
      CTRL_REG1_G_ODR2          = 0x40, // DR0
      CTRL_REG1_G_ODR3          = 0x80, // DR1
      _CTRL_REG1_G_ODR_MASK     = 15,
      _CTRL_REG1_G_ODR_SHIFT    = 4
    } CTRL_REG1_G_BITS_T;

    /**
     * CRTL_REG1_G_ODR values
     */
    typedef enum {
      G_ODR_95_12_5             = 0, // ODR = 95Hz, cutoff = 12.5
      G_ODR_95_25               = 1, // ODR = 95Hz, cutoff = 25
      // Other two (2 and 3) are the same (95_25)

      G_ODR_190_12_5            = 4,
      G_ODR_190_25              = 5,
      G_ODR_190_50              = 6,
      G_ODR_190_70              = 7,

      G_ODR_380_20              = 8,
      G_ODR_380_25              = 9,
      G_ODR_380_50              = 10,
      G_ODR_380_100             = 11,

      G_ODR_760_30              = 12,
      G_ODR_760_35              = 13,
      G_ODR_760_50              = 14,
      G_ODR_760_100             = 15
    } G_ODR_T;

    /**
     * Gyro CTRL_REG2_G bits
     */
    typedef enum {
      CTRL_REG2_G_HPCF0         = 0x01, // high-pass cutoff freq
      CTRL_REG2_G_HPCF1         = 0x02,
      CTRL_REG2_G_HPCF2         = 0x04,
      CTRL_REG2_G_HPCF3         = 0x08,
      _CTRL_REG2_G_HPCF_MASK    = 15,
      _CTRL_REG2_G_HPCF_SHIFT   = 0,

      CTRL_REG2_G_HPM0          = 0x10, // high-pass filter mode
      CTRL_REG2_G_HPM1          = 0x20,
      _CTRL_REG2_G_HPM_MASK     = 3,
      _CTRL_REG2_G_HPM_SHIFT    = 4,

      // 0x40, 0x80 reserved
    } CTRL_REG2_G_BITS_T;

    /**
     * CRTL_REG2_G_HPCF values
     *
     * See table 26 in the datasheet, as these depend on your data
     * rate (ODR).  We will label these according to the 95Hz column,
     * but of course the actual cutoff frequency depends on ODR.
     */
    typedef enum {
      G_HPCF_7_2                = 0, // 7.2 Hz (if ODR is 95Hz)
      G_HPCF_3_5                = 1,
      G_HPCF_1_8                = 2,
      G_HPCF_0_9                = 3, // 0.9Hz
      G_HPCF_0_45               = 4,
      G_HPCF_0_18               = 5,
      G_HPCF_0_09               = 6,
      G_HPCF_0_045              = 7,
      G_HPCF_0_018              = 8,
      G_HPCF_0_009              = 9

      // 10-15 unused
    } G_HPCF_T;

    /**
     * CRTL_REG2_G_HPM values
     *
     */
    typedef enum {
      G_HPM_NORMAL_RESET_HPF    = 0, // reset reading (HP_RESET_FILTER)
      G_HPM_REFERENCE           = 1, // REF signal for filtering
      G_HPM_NORMAL              = 2, // normal mode
      G_HPM_AUTORESET_ON_INTR   = 3  // autoreset in interrupt event
    } G_HPM_T;

    /**
     * Gyro CTRL_REG3_G bits (interrupt G config)
     */
    typedef enum {
      CTRL_REG3_G_I2_EMPTY      = 0x01, // FIFO empty on DRDY_G
      CTRL_REG3_G_I2_ORUN       = 0x02, // FIFO Overrun intr
      CTRL_REG3_G_I2_WTM        = 0x04, // FIFO watermark intr
      CTRL_REG3_G_I2_DRDY       = 0x08, // data ready on DRDY_G
      CTRL_REG3_G_PP_OD         = 0x10, // push-pull/open drain
      CTRL_REG3_G_H_LACTIVE     = 0x20,
      CTRL_REG3_G_I1_BOOT       = 0x40,
      CTRL_REG3_G_I1_INT1       = 0x80, // intr enable on INT_G pin
    } CTRL_REG3_G_BITS_T;

    /**
     * Gyro CTRL_REG4_G bits
     */
    typedef enum {
      CTRL_REG4_G_SIM           = 0x01, // SPI mode selection

      CTRL_REG4_G_ST0           = 0x02, // self test enables
      CTRL_REG4_G_ST1           = 0x04,
      _CTRL_REG4_G_ST_MASK      = 3,
      _CTRL_REG4_G_ST_SHIFT     = 1,

      // 0x08 reserved

      CTRL_REG4_G_FS0           = 0x10, // full scale selection
      CTRL_REG4_G_FS1           = 0x20,
      _CTRL_REG4_G_FS_MASK      = 3,
      _CTRL_REG4_G_FS_SHIFT     = 4,

      CTRL_REG4_G_BLE           = 0x40, // big/little endian data selection
      CTRL_REG4_G_BDU           = 0x80  // block data updates
    } CTRL_REG4_G_BITS_T;

    /**
     * CRTL_REG4_G_ST values
     *
     */
    typedef enum {
      G_ST_NORMAL               = 0, // normal mode
      G_ST_SELFTEST0            = 1, // x+, y-, z-

      // 2, reserved

      G_ST_SELFTEST1            = 3  // x-, y+, z+
    } G_ST_T;

    /**
     * CRTL_REG4_G_FS values
     *
     */
    typedef enum {
      G_FS_245                  = 0, // 245 deg/sec
      G_FS_500                  = 1,
      G_FS_2000                 = 2
      // 3 is also 2000
    } G_FS_T;

    /**
     * Gyro CTRL_REG5_G bits
     */
    typedef enum {
      CTRL_REG5_G_OUTSEL0       = 0x01, // see fig. 18 in the datasheet
      CTRL_REG5_G_OUTSEL1       = 0x02,
      _CTRL_REG5_G_OUTSEL_MASK  = 3,
      _CTRL_REG5_G_OUTSEL_SHIFT = 0,

      CTRL_REG5_G_INT1SEL0      = 0x04, // see fig. 18 in the datasheet
      CTRL_REG5_G_INT1SEL1      = 0x08,
      _CTRL_REG5_G_INT1SEL_MASK = 3,
      _CTRL_REG5_G_INT1SEL_SHIFT = 2,

      CTRL_REG5_G_HPEN          = 0x10, // HPF enable

      // 0x20 reserved

      CTRL_REG5_G_FIFO_EN       = 0x40,
      CTRL_REG5_G_BOOT          = 0x80  // reboot memory content
    } CTRL_REG5_G_BITS_T;

    
    /**
     * CRTL_REG5_G_OUTSEL and INT1SEL values.  See Figure 18 in the
     * datasheet.
     */
    typedef enum {
      G_INT1OUTSEL_0            = 0,
      G_INT1OUTSEL_1            = 1,
      G_INT1OUTSEL_2            = 2,
      G_INT1OUTSEL_3            = 3
    } G_INT1OUTSEL_T;

    /**
     * Gyro STATUS_REG_G bits
     */
    typedef enum {
      STATUS_REG_G_XDA          = 0x01, // X axis data available
      STATUS_REG_G_YDA          = 0x02,
      STATUS_REG_G_ZDA          = 0x04,
      STATUS_REG_G_ZYXDA        = 0x08, // X, Y, and Z data available

      STATUS_REG_G_XOR          = 0x10, // X data overrun
      STATUS_REG_G_YOR          = 0x20,
      STATUS_REG_G_ZOR          = 0x40,
      STATUS_REG_G_ZYXOR        = 0x80
    } STATUS_REG_G_BITS_T;

    /**
     * Gyro FIFO_CTRL_REG_G bits
     */
    typedef enum {
      FIFO_CTRL_REG_G_WTM0      = 0x01, // FIFO watermark
      FIFO_CTRL_REG_G_WTM1      = 0x02,
      FIFO_CTRL_REG_G_WTM2      = 0x04,
      FIFO_CTRL_REG_G_WTM3      = 0x08,
      FIFO_CTRL_REG_G_WTM4      = 0x10,
      _FIFO_CTRL_REG_G_WTM_MASK = 31,
      _FIFO_CTRL_REG_G_WTM_SHIFT = 0,
      
      FIFO_CTRL_REG_G_FM0       = 0x20, // FIFO mode config
      FIFO_CTRL_REG_G_FM1       = 0x40,
      FIFO_CTRL_REG_G_FM2       = 0x80,
      _FIFO_CTRL_REG_G_FM_MASK  = 7,
      _FIFO_CTRL_REG_G_FM_SHIFT = 5,
    } FIFO_CTRL_REG_G_T;

    // FIFO_CTRL_REG_G_WTM (FIFO watermark) is just a numeric value
    // between 0-31, so we won't enumerate those values.

    /**
     * FIFO_CTRL_REG_G_FM values (FIFO Modes)
     *
     */
    typedef enum {
      G_FM_BYPASS               = 0,
      G_FM_FIFO                 = 1,
      G_FM_STREAM               = 2,
      G_FM_STREAM2FIFO          = 3,
      G_FM_BYPASS2STREAM        = 4

      // 5-7 unused
    } G_FM_T;

    /**
     * FIFO_SRC_REG_G bits
     *
     */
    typedef enum {
      FIFO_CTRL_REG_G_FSS0      = 0x01, // FIFO stored data level
      FIFO_CTRL_REG_G_FSS1      = 0x02,
      FIFO_CTRL_REG_G_FSS2      = 0x04,
      FIFO_CTRL_REG_G_FSS3      = 0x08,
      FIFO_CTRL_REG_G_FSS4      = 0x10,
      _FIFO_CTRL_REG_G_FSS_MASK = 31,
      _FIFO_CTRL_REG_G_FSS_SHIFT = 0,

      FIFO_CTRL_REG_G_EMPTY     = 0x20, // FIFO empty
      FIFO_CTRL_REG_G_OVRN      = 0x40, // FIFO overrun
      FIFO_CTRL_REG_G_WTM       = 0x80  // watermark status
    } FIFO_SRC_REG_G_BITS_T;

    /**
     * INT1_CFG_G bits
     *
     */
    typedef enum {
      INT1_CFG_G_XLIE           = 0x01, // X Low event interrupt enable
      INT1_CFG_G_XHIE           = 0x02, // X High event interrupt enable
      INT1_CFG_G_YLIE           = 0x04,
      INT1_CFG_G_YHIE           = 0x08,
      INT1_CFG_G_ZLIE           = 0x10,
      INT1_CFG_G_ZHIE           = 0x20,

      INT1_CFG_G_LIR            = 0x40, // latch interrupt request
      INT1_CFG_G_ANDOR          = 0x80  // OR or AND interrupt events
    } INT1_CFG_G_BITS_T;

    /**
     * INT1_SRC_G bits
     *
     */
    typedef enum {
      INT1_SRC_G_XL             = 0x01, // X low interrupt
      INT1_SRC_G_XH             = 0x02, // X high interrupt
      INT1_SRC_G_YL             = 0x04,
      INT1_SRC_G_YH             = 0x08,
      INT1_SRC_G_ZL             = 0x10,
      INT1_SRC_G_ZH             = 0x20,

      INT1_SRC_G_IA             = 0x40 // interrupt active

      // 0x80 reserved
    } INT1_SRC_G_BITS_T;

    // The following registers are for the Accelerometer (A/X),
    // Magnetometer (M), and Temperature device.

    /**
     * LSM9DS0 Accelerometer (X) and Magnetometer (M) registers
     */
    typedef enum {
      // 0x00-0x04 reserved

      REG_OUT_TEMP_L_XM         = 0x05, // temperature
      REG_OUT_TEMP_H_XM         = 0x06,

      REG_STATUS_REG_M          = 0x07,

      REG_OUT_X_L_M             = 0x08, // magnetometer outputs
      REG_OUT_X_H_M             = 0x09,
      REG_OUT_Y_L_M             = 0x0a,
      REG_OUT_Y_H_M             = 0x0b,
      REG_OUT_Z_L_M             = 0x0c,
      REG_OUT_Z_H_M             = 0x0d,

      // 0x0e reserved

      REG_WHO_AM_I_XM           = 0x0f,

      // 0x10, 0x11 reserved

      REG_INT_CTRL_REG_M        = 0x12,
      REG_INT_SRC_REG_M         = 0x13,

      REG_INT_THS_L_M           = 0x14, // magnetometer threshold
      REG_INT_THS_H_M           = 0x15,

      REG_OFFSET_X_L_M          = 0x16,
      REG_OFFSET_X_H_M          = 0x17,
      REG_OFFSET_Y_L_M          = 0x18,
      REG_OFFSET_Y_H_M          = 0x19,
      REG_OFFSET_Z_L_M          = 0x1a,
      REG_OFFSET_Z_H_M          = 0x1b,

      REG_REFERENCE_X           = 0x1c,
      REG_REFERENCE_Y           = 0x1d,
      REG_REFERENCE_Z           = 0x1e,

      REG_CTRL_REG0_XM          = 0x1f,
      REG_CTRL_REG1_XM          = 0x20,
      REG_CTRL_REG2_XM          = 0x21,
      REG_CTRL_REG3_XM          = 0x22,
      REG_CTRL_REG4_XM          = 0x23,
      REG_CTRL_REG5_XM          = 0x24,
      REG_CTRL_REG6_XM          = 0x25,
      REG_CTRL_REG7_XM          = 0x26,

      REG_STATUS_REG_A          = 0x27,

      REG_OUT_X_L_A             = 0x28, // accelerometer outputs
      REG_OUT_X_H_A             = 0x29,
      REG_OUT_Y_L_A             = 0x2a,
      REG_OUT_Y_H_A             = 0x2b,
      REG_OUT_Z_L_A             = 0x2c,
      REG_OUT_Z_H_A             = 0x2d,

      REG_FIFO_CTRL_REG         = 0x2e,
      REG_FIFO_SRC_REG          = 0x2f,

      REG_INT_GEN_1_REG         = 0x30,
      REG_INT_GEN_1_SRC         = 0x31,
      REG_INT_GEN_1_THS         = 0x32,
      REG_INT_GEN_1_DURATION    = 0x33,

      REG_INT_GEN_2_REG         = 0x34,
      REG_INT_GEN_2_SRC         = 0x35,
      REG_INT_GEN_2_THS         = 0x36,
      REG_INT_GEN_2_DURATION    = 0x37,

      REG_CLICK_CFG             = 0x38,
      REG_CLICK_SRC             = 0x39,
      REG_CLICK_THS             = 0x3a,

      REG_TIME_LIMIT            = 0x3b,
      REG_TIME_LATENCY          = 0x3c,
      REG_TIME_WINDOW           = 0x3d,
      
      REG_ACT_THS               = 0x3e,
      REG_ACT_DUR               = 0x3f
    } REG_XM_T;

    /**
     * XM STATUS_REG_M bits
     */
    typedef enum {
      STATUS_REG_M_XMDA         = 0x01, // X mag axis data available
      STATUS_REG_M_YMDA         = 0x02,
      STATUS_REG_M_ZMDA         = 0x04,
      STATUS_REG_M_ZYXMDA       = 0x08, // X, Y, and Z mag data available

      STATUS_REG_M_XMOR         = 0x10, // X mag data overrun
      STATUS_REG_M_YMOR         = 0x20,
      STATUS_REG_M_ZMOR         = 0x40,
      STATUS_REG_M_ZYXMOR       = 0x80
    } STATUS_REG_M_BITS_T;

    /**
     * INT_CTRL_REG_M bits
     */
    typedef enum {
      INT_CTRL_REG_M_MIEN       = 0x01, // mag interrupt enable
      INT_CTRL_REG_M_4D         = 0x02,
      INT_CTRL_REG_M_IEL        = 0x04, // latch intr request
      INT_CTRL_REG_M_IEA        = 0x08,
      INT_CTRL_REG_M_PP_OD      = 0x10, // push-pull/open drian 
      INT_CTRL_REG_M_ZMIEN      = 0x20, // Z mag axis interrupt recognition
      INT_CTRL_REG_M_YMIEN      = 0x40,
      INT_CTRL_REG_M_XMIEN      = 0x80
    } INT_CTRL_REG_M_BITS_T;

    /**
     * INT_SRC_REG_M bits
     */
    typedef enum {
      INT_SRC_REG_M_MINT        = 0x01,
      INT_SRC_REG_M_MROI        = 0x02,
      INT_SRC_REG_M_NTH_Z       = 0x04,
      INT_SRC_REG_M_NTH_Y       = 0x08,
      INT_SRC_REG_M_NTH_X       = 0x10,
      INT_SRC_REG_M_PTH_Z       = 0x20,
      INT_SRC_REG_M_PTH_Y       = 0x40,
      INT_SRC_REG_M_PTH_X       = 0x80
    } INT_SRC_REG_M_BITS_T;


    /**
     * CTRL_REG0_XM bits
     */
    typedef enum {
      CTRL_REG0_XM_HPIS2        = 0x01, // HPF enable for int generator 2
      CTRL_REG0_XM_HPIS1        = 0x02,

      CTRL_REG0_XM_HP_CLICK     = 0x04, // HPF enable for click

      // 0x08,0x10 reserved

      CTRL_REG0_XM_WTM_LEN      = 0x20, // watermark enable
      CTRL_REG0_XM_FIFO_EN      = 0x40, // FIFO enable
      CTRL_REG0_XM_BOOT         = 0x80  // reboot memory content
    } CTRL_REG0_XM_BITS_T;

    /**
     * CTRL_REG1_XM bits
     */
    typedef enum {
      CTRL_REG1_XM_AXEN         = 0x01, // accelerometer x axis enable
      CTRL_REG1_XM_AYEN         = 0x02,
      CTRL_REG1_XM_AZEN         = 0x03,

      CTRL_REG1_XM_BDU          = 0x04, // block data update

      CTRL_REG1_XM_AODR0        = 0x10, // accelerometer output data rate
      CTRL_REG1_XM_AODR1        = 0x20,      
      CTRL_REG1_XM_AODR2        = 0x40,
      CTRL_REG1_XM_AODR3        = 0x80,
      _CTRL_REG1_XM_AODR_MASK   = 15,
      _CTRL_REG1_XM_AODR_SHIFT  = 4
    } CTRL_REG1_XM_BITS_T;

    /**
     * CTRL_REG1_XM_AODR values
     */
    typedef enum {
      XM_AODR_PWRDWN            = 0, // power down mode
      XM_AODR_3_125             = 1, // 3.125 Hz
      XM_AODR_6_25              = 2,
      XM_AODR_12_5              = 3,
      XM_AODR_25                = 4, // 25Hz
      XM_AODR_50                = 5,
      XM_AODR_100               = 6,
      XM_AODR_200               = 7,
      XM_AODR_400               = 8,
      XM_AODR_800               = 9,
      XM_AODR_1000              = 10
      // 11-15 unused
    } XM_AODR_T;
    
    /**
     * CTRL_REG2_XM bits
     */
    typedef enum {
      CTRL_REG2_XM_SIM         = 0x01,

      CTRL_REG2_XM_AST0        = 0x02, // accel self-test enable
      CTRL_REG2_XM_AST1        = 0x04,
      _CTRL_REG2_XM_AST_MASK   = 3,
      _CTRL_REG2_XM_AST_SHIFT  = 1,

      CTRL_REG2_XM_AFS0        = 0x08, // accel full scale
      CTRL_REG2_XM_AFS1        = 0x10,
      CTRL_REG2_XM_AFS2        = 0x20,
      _CTRL_REG2_XM_AFS_MASK   = 7,
      _CTRL_REG2_XM_AFS_SHIFT  = 3,

      CTRL_REG2_XM_ABW0        = 0x40, // accel anti-alias filter bandwidth
      CTRL_REG2_XM_ABW1        = 0x80,
      _CTRL_REG2_XM_ABW_MASK   = 3,
      _CTRL_REG2_XM_ABW_SHIFT  = 6
    } CTRL_REG2_XM_BITS_T;

    /**
     * CTRL_REG2_XM_AST values
     */
    typedef enum {
      XM_AST_NORMAL            = 0,
      XM_AST_POS_SIGN          = 1,
      XM_AST_NEG_SIGN          = 2
      // 3 not allowed
    } XM_AST_T;

    /**
     * CTRL_REG2_XM_AFS (accel full scale) values
     */
    typedef enum {
      XM_AFS_2                 = 0, // 2g
      XM_AFS_4                 = 1,
      XM_AFS_6                 = 2,
      XM_AFS_8                 = 3,
      XM_AFS_16                = 4

      // 5-7 not used
    } XM_AFS_T;

    /**
     * CTRL_REG2_XM_ABW (accel anti-alias filter bandwidth) values
     */
    typedef enum {
      XM_ABW_773               = 0, // 773Hz
      XM_ABW_194               = 1, // these two might be inverted (typo in ds)
      XM_ABW_362               = 2,
      XM_ABW_50                = 3
    } XM_ABW_T;

    /**
     * CTRL_REG3_XM bits
     */
    typedef enum {
      CTRL_REG3_XM_P1_EMPTY    = 0x01, // INT1_XM pin enables
      CTRL_REG3_XM_P1_DRDYM    = 0x02,
      CTRL_REG3_XM_P1_DRDYA    = 0x04,
      CTRL_REG3_XM_P1_INTM     = 0x08,
      CTRL_REG3_XM_P1_INT2     = 0x10,
      CTRL_REG3_XM_P1_INT1     = 0x20,
      CTRL_REG3_XM_P1_TAP      = 0x40,
      CTRL_REG3_XM_P1_BOOT     = 0x80
    } CTRL_REG3_XM_BITS_T;

    /**
     * CTRL_REG4_XM bits
     */
    typedef enum {
      CTRL_REG4_XM_P2_WTM      = 0x01, // INT2_XM pin enables
      CTRL_REG4_XM_P2_OVERRUN  = 0x02,
      CTRL_REG4_XM_P2_DRDYM    = 0x04,
      CTRL_REG4_XM_P2_DRDYA    = 0x08,
      CTRL_REG4_XM_P2_INTM     = 0x10,
      CTRL_REG4_XM_P2_INT2     = 0x20,
      CTRL_REG4_XM_P2_INT1     = 0x40,
      CTRL_REG4_XM_P2_TAP      = 0x80
    } CTRL_REG4_XM_BITS_T;

    /**
     * CTRL_REG5_XM bits
     */
    typedef enum {
      CTRL_REG5_XM_LIR1        = 0x01, // latch intr 1
      CTRL_REG5_XM_LIR2        = 0x02, // latch intr 2

      CTRL_REG5_XM_ODR0        = 0x04, // mag output data rate
      CTRL_REG5_XM_ODR1        = 0x08,
      CTRL_REG5_XM_ODR2        = 0x10,
      _CTRL_REG5_XM_ODR_MASK   = 7,
      _CTRL_REG5_XM_ODR_SHIFT  = 2,

      CTRL_REG5_XM_RES0        = 0x20, // mag resolution
      CTRL_REG5_XM_RES1        = 0x40,
      _CTRL_REG5_XM_RES_MASK   = 3,
      _CTRL_REG5_XM_RES_SHIFT  = 5,

      CTRL_REG5_XM_TEMP_EN     = 0x80  // temp sensor enable
    } CTRL_REG5_XM_BITS_T;

    /**
     * CTRL_REG5_XM_ODR (magnetometer output data rate) values
     */
    typedef enum {
      XM_ODR_3_125             = 0, // 3.125Hz
      XM_ODR_6_25              = 1,
      XM_ODR_12_5              = 2,
      XM_ODR_25                = 3,
      XM_ODR_50                = 4,
      XM_ODR_100               = 5
      
      // 6, 7 reserved
    } XM_ODR_T;

    /**
     * CTRL_REG5_XM_RES (magnetometer resolution) values
     */
    typedef enum {
      XM_RES_LOW               = 0, // low resolution
      
      // 1, 2 reserved
      
      XM_RES_HIGH              = 3,
    } XM_RES_T;

    /**
     * CTRL_REG6_XM bits
     */
    typedef enum {
      // 0x01-0x10 reserved

      CTRL_REG6_XM_MFS0        = 0x20,
      CTRL_REG6_XM_MFS1        = 0x40,
      _CTRL_REG6_XM_MFS_MASK   = 3,
      _CTRL_REG6_XM_MFS_SHIFT  = 5

      // 0x80 reserved
    } CTRL_REG6_XM_BITS_T;

    /**
     * CTRL_REG6_XM_MFS (magnetometer full scale) values
     */
    typedef enum {
      XM_MFS_2                 = 0, // +/- 2 gauss
      XM_MFS_4                 = 1,
      XM_MFS_8                 = 2,
      XM_MFS_12                = 3
    } XM_MFS_T;

    /**
     * CTRL_REG7_XM bits
     */
    typedef enum {
      CTRL_REG7_XM_MD0         = 0x01, // mag sensor mode
      CTRL_REG7_XM_MD1         = 0x02,
      _CTRL_REG7_XM_MD_MASK    = 3,
      _CTRL_REG7_XM_MD_SHIFT   = 0,

      CTRL_REG7_XM_MLP         = 0x04, // mag low power mode

      // 0x08, 0x10 reserved

      CTRL_REG7_XM_AFDS        = 0x20, // filtered acceleration data

      CTRL_REG7_XM_AHPM0       = 0x40, // accel HPF selection
      CTRL_REG7_XM_AHPM1       = 0x80,
      _CTRL_REG7_XM_AHPM_MASK  = 3,
      _CTRL_REG7_XM_AHPM_SHIFT = 6
    } CTRL_REG7_XM_BITS_T;

    /**
     * CTRL_REG7_XM_MD (magnetometer sensor mode) values
     */
    typedef enum {
      XM_MD_CONTINUOUS         = 0, // continuous conversion
      XM_MD_SINGLE             = 1, // single conversion
      XM_MD_POWERDOWN          = 2  // power down mode
      // 3 is also power down mode, for some odd reason
    } XM_MD_T;

    /**
     * CTRL_REG7_AHPM_MD (accel high-pass filter mode) values
     */
    typedef enum {
      // XM_AHPM_NORMAL_REF: Normal mode (resets x, y and z-axis
      // reading REFERENCE_X (1Ch), REFERENCE_Y (1Dh) and REFERENCE_Y
      // (1Dh) registers respectively)

      XM_AHPM_NORMAL_REF       = 0,
      XM_AHPM_REFERENCE        = 1,
      XM_AHPM_NORMAL           = 2,
      XM_AHPM_AUTORESET        = 3  // autoreset on interrupt
    } XM_AHPM_T;

    /**
     * XM STATUS_REG_A bits
     */
    typedef enum {
      STATUS_REG_A_XADA         = 0x01, // X accel axis data available
      STATUS_REG_A_YADA         = 0x02,
      STATUS_REG_A_ZADA         = 0x04,
      STATUS_REG_A_ZYXADA       = 0x08, // X, Y, and Z accel data available

      STATUS_REG_A_XAOR         = 0x10, // X accel data overrun
      STATUS_REG_A_YAOR         = 0x20,
      STATUS_REG_A_ZAOR         = 0x40,
      STATUS_REG_A_ZYXAOR       = 0x80
    } STATUS_REG_A_BITS_T;

    /**
     * XM FIFO_CTRL_REG bits
     */
    typedef enum {
      FIFO_CTRL_REG_FTH0        = 0x01, // FIFO watermark/threshold
      FIFO_CTRL_REG_FTH1        = 0x02,
      FIFO_CTRL_REG_FTH2        = 0x04,
      FIFO_CTRL_REG_FTH3        = 0x08,
      FIFO_CTRL_REG_FTH4        = 0x10,
      _FIFO_CTRL_REG_FTH_MASK   = 31,
      _FIFO_CTRL_REG_FTH_SHIFT  = 0,
      
      FIFO_CTRL_REG_FM0         = 0x20, // FIFO mode config
      FIFO_CTRL_REG_FM1         = 0x40,
      FIFO_CTRL_REG_FM2         = 0x80,
      _FIFO_CTRL_REG_FM_MASK    = 7,
      _FIFO_CTRL_REG_FM_SHIFT   = 5,
    } FIFO_CTRL_REG_T;

    // FIFO_CTRL_REG_FTH (FIFO watermark/threshold) is just a numeric
    // value between 0-31, so we won't enumerate those values.

    /**
     * XM FIFO_CTRL_REG_FM values (FIFO Modes)
     *
     */
    typedef enum {
      FM_BYPASS                 = 0,
      FM_FIFO                   = 1,
      FM_STREAM                 = 2,
      FM_STREAM2FIFO            = 3,
      FM_BYPASS2STREAM          = 4

      // 5-7 unused
    } FM_T;

    /**
     * FIFO_SRC_REG bits
     *
     */
    typedef enum {
      FIFO_CTRL_REG_FSS0        = 0x01, // FIFO stored data level
      FIFO_CTRL_REG_FSS1        = 0x02,
      FIFO_CTRL_REG_FSS2        = 0x04,
      FIFO_CTRL_REG_FSS3        = 0x08,
      FIFO_CTRL_REG_FSS4        = 0x10,
      _FIFO_CTRL_REG_FSS_MASK   = 31,
      _FIFO_CTRL_REG_FSS_SHIFT  = 0,

      FIFO_CTRL_REG_EMPTY       = 0x20, // FIFO empty
      FIFO_CTRL_REG_OVRN        = 0x40, // FIFO overrun
      FIFO_CTRL_REG_WTM         = 0x80  // watermark status
    } FIFO_SRC_REG_BITS_T;

    /**
     * INT_GEN_1_REG and INT_GEN_2_REG (GEN_X) bits
     *
     */
    typedef enum {
      INT_GEN_X_REG_XLIE_XDOWNE = 0x01, // enable intr on X low or dir recog
      INT_GEN_X_REG_XHIE_XUPE   = 0x02,
      INT_GEN_X_REG_YLIE_YDOWNE = 0x04,
      INT_GEN_X_REG_YHIE_YUPE   = 0x08,
      INT_GEN_X_REG_ZLIE_ZDOWNE = 0x10,
      INT_GEN_X_REG_ZHIE_ZUPE   = 0x20,
      INT_GEN_X_REG_6D          = 0x40, // enable 6D direction function
      INT_GEN_X_REG_AOI         = 0x80  // AND/OR combination of intrs
    } INT_GEN_X_REG_BITS_T;

    /**
     * INT_GEN_1_SRC and INT_GEN_2_SRC (GEN_X) bits
     *
     */
    typedef enum {
      INT_GEN_X_SRC_XL          = 0x01,
      INT_GEN_X_SRC_XH          = 0x02,
      INT_GEN_X_SRC_YL          = 0x04,
      INT_GEN_X_SRC_YH          = 0x08,
      INT_GEN_X_SRC_ZL          = 0x10,
      INT_GEN_X_SRC_ZH          = 0x20,
      INT_GEN_X_SRC_IA          = 0x40
      // 0x80 reserved
    } INT_GEN_X_SRC_BITS_T;

    /**
     * INT_GEN_1_THS and INT_GEN_2_THS (GEN_X) bits
     *
     */
    typedef enum {
      INT_GEN_X_THS0            = 0x01, // interrupt threshold
      INT_GEN_X_THS1            = 0x02,
      INT_GEN_X_THS2            = 0x04,
      INT_GEN_X_THS3            = 0x08,
      INT_GEN_X_THS4            = 0x10,
      INT_GEN_X_THS5            = 0x20,
      INT_GEN_X_THS6            = 0x40,
      _INT_GEN_X_THS_MASK       = 127,
      _INT_GEN_X_THS_SHIFT      = 0
      // 0x80 reserved
    } INT_GEN_X_THS_BITS_T;

    /**
     * INT_GEN_1_DUR and INT_GEN_2_DUR (GEN_X) bits
     *
     */
    typedef enum {
      INT_GEN_X_DUR0            = 0x01, // interrupt duration
      INT_GEN_X_DUR1            = 0x02,
      INT_GEN_X_DUR2            = 0x04,
      INT_GEN_X_DUR3            = 0x08,
      INT_GEN_X_DUR4            = 0x10,
      INT_GEN_X_DUR5            = 0x20,
      INT_GEN_X_DUR6            = 0x40,
      _INT_GEN_X_DUR_MASK       = 127,
      _INT_GEN_X_DUR_SHIFT      = 0
      // 0x80 reserved
    } INT_GEN_X_DUR_BITS_T;

    /**
     * CLICK_CONFIG bits
     *
     */
    typedef enum {
      CLICK_CONFIG_XS           = 0x01, // enable intr single click x
      CLICK_CONFIG_XD           = 0x02, // enable intr double click x
      CLICK_CONFIG_YS           = 0x04,
      CLICK_CONFIG_YD           = 0x08,
      CLICK_CONFIG_ZS           = 0x10,
      CLICK_CONFIG_ZD           = 0x20
      // 0x40, 0x80 reserved
    } CLICK_CONFIG_BITS_T;
    
    /**
     * CLICK_SRC bits
     *
     */
    typedef enum {
      CLICK_SRC_X               = 0x01,
      CLICK_SRC_Y               = 0x02,
      CLICK_SRC_Z               = 0x04,
      CLICK_SRC_SIGN            = 0x08,
      CLICK_SRC_SCLICK          = 0x10,
      CLICK_SRC_DCLICK          = 0x20,
      CLICK_SRC_IA              = 0x40
      // 0x80 reserved
    } CLICK_SRC_BITS_T;

    /**
     * CLICK_THS bits
     *
     */
    typedef enum {
      CLICK_THS_THS0            = 0x01, // click threshold
      CLICK_THS_THS1            = 0x02,
      CLICK_THS_THS2            = 0x04,
      CLICK_THS_THS3            = 0x08,
      CLICK_THS_THS4            = 0x10,
      CLICK_THS_THS5            = 0x20,
      CLICK_THS_THS6            = 0x40,
      _CLICK_THS_THS_MASK       = 127,
      _CLICK_THS_THS_SHIFT      = 0
      // 0x80 reserved
    } CLICK_THS_BITS_T;

    /**
     * CLICK_TIME_LIMIT bits
     *
     */
    typedef enum {
      CLICK_TIME_LIMIT_TLI0     = 0x01,
      CLICK_TIME_LIMIT_TLI1     = 0x02,
      CLICK_TIME_LIMIT_TLI2     = 0x04,
      CLICK_TIME_LIMIT_TLI3     = 0x08,
      CLICK_TIME_LIMIT_TLI4     = 0x10,
      CLICK_TIME_LIMIT_TLI5     = 0x20,
      CLICK_TIME_LIMIT_TLI6     = 0x40,
      _CLICK_TIME_LIMIT_TLI_MASK = 127,
      _CLICK_TIME_LIMIT_TLI_SHIFT = 0
      // 0x80 reserved
    } CLICK_TIME_LIMIT_BITS_T;

    /**
     * ACT_THS (sleep-to-wake/return-to-sleep activation threshold) bits
     *
     */
    typedef enum {
      ACT_THS_ACTH0             = 0x01, // 1 LSb = 16mg (?)
      ACT_THS_ACTH1             = 0x02,
      ACT_THS_ACTH2             = 0x04,
      ACT_THS_ACTH3             = 0x08,
      ACT_THS_ACTH4             = 0x10,
      ACT_THS_ACTH5             = 0x20,
      ACT_THS_ACTH6             = 0x40,
      _ACT_THS_ACTH_MASK        = 127,
      _ACT_THS_ACTH_SHIFT       = 0
      // 0x80 reserved
    } ACT_THS_BITS_T;

    // Driver specific enumerations

    // device enums for read/write regs
    typedef enum {
      DEV_GYRO,
      DEV_XM
    } DEVICE_T;

    // interrupt selection for installISR() and uninstallISR()
    typedef enum {
      INTERRUPT_G_INT,  // gyroscope interrupt
      INTERRUPT_G_DRDY, // gyroscope data ready interrupt
      INTERRUPT_XM_GEN1, // XM interrupt generator 1
      INTERRUPT_XM_GEN2  // XM interrupt generator 2
    } INTERRUPT_PINS_T;


    /**
     * lsm9ds0 constructor
     *
     * @param bus i2c bus to use
     * @param address the address for this device
     */
    LSM9DS0(int bus=LSM9DS0_I2C_BUS, 
            uint8_t gAddress=LSM9DS0_DEFAULT_GYRO_ADDR,
            uint8_t xmAddress=LSM9DS0_DEFAULT_XM_ADDR);

    /**
     * LSM9DS0 Destructor
     */
    ~LSM9DS0();
    
    /**
     * set up initial values and start operation
     *
     * @return true if successful
     */
    bool init();

    /**
     * update the accelerometer, gyroscope, magnetometer and
     * termperature values.
     */
    void update();

    /**
     * update the gyroscope values only
     */
    void updateGyroscope();

    /**
     * update the accelerometer values only
     */
    void updateAccelerometer();

    /**
     * update the magnetometer values only
     */
    void updateMagnetometer();

    /**
     * update the temperature value only
     */
    void updateTemperature();

    /**
     * read a register
     *
     * @param dev the device to access (XM or G)
     * @param reg the register to read
     * @return the value of the register
     */
    uint8_t readReg(DEVICE_T dev, uint8_t reg);

    /**
     * read contiguous register into a buffer
     *
     * @param dev the device to access (XM or G)
     * @param reg the register to start reading at
     * @param buf the buffer to store the results
     * @param len the number of registers to read
     * @return the value of the register
     */
    void readRegs(DEVICE_T dev, uint8_t reg, uint8_t *buffer, int len);

    /**
     * write to a register
     *
     * @param dev the device to access (XM or G)
     * @param reg the register to write to
     * @param val the value to write
     * @return true if successful, false otherwise
     */
    bool writeReg(DEVICE_T dev, uint8_t reg, uint8_t val);

    /**
     * enable or disable the gyro power down mode
     *
     * @param enable true to put device to sleep, false to wake up
     * @return true if successful, false otherwise
     */
    bool setGyroscopePowerDown(bool enable);

    /**
     * enable or disable gyroscope axes.  If all axis are disabled,
     * and powerdown mode is not set, then the gyro goes into sleep
     * mode.
     *
     * @param axes bit mask of valid axes, (CTRL_REG1_G_YEN, ...)
     * @return true if successful, false otherwise
     */
    bool setGyroscopeEnableAxes(uint8_t axes);

    /**
     * set the gyroscope Output Data Rate (ODR)
     *
     * @param odr one of the G_ODR_T values
     * @return true if successful, false otherwise
     */
    bool setGyroscopeODR(G_ODR_T odr);

    /**
     * set the scaling mode of the gyroscope
     *
     * @param scale one of the G_FS_T values
     * @return true if successful, false otherwise
     */
    bool setGyroscopeScale(G_FS_T scale);

    /**
     * enable or disable accelerometer axes.
     *
     * @param axes bit mask of valid axes, (CTRL_REG1_XM_AXEN, ...)
     * @return true if successful, false otherwise
     */
    bool setAccelerometerEnableAxes(uint8_t axes);

    /**
     * set the accelerometer Output Data Rate (ODR)
     *
     * @param odr one of the XM_AODR_T values
     * @return true if successful, false otherwise
     */
    bool setAccelerometerODR(XM_AODR_T odr);

    /**
     * set the scaling mode of the accelerometer
     *
     * @param scale one of the XM_AFS_T values
     * @return true if successful, false otherwise
     */
    bool setAccelerometerScale(XM_AFS_T scale);

    /**
     * set the magnetometer resolution
     *
     * @param res one of the XM_RES_T values
     * @return true if successful, false otherwise
     */
    bool setMagnetometerResolution(XM_RES_T res);

    /**
     * set the magnetometer Output Data Rate (ODR)
     *
     * @param odr one of the XM_ODR_T values
     * @return true if successful, false otherwise
     */
    bool setMagnetometerODR(XM_ODR_T odr);

    /**
     * set the magnetometer sensor mode
     *
     * @param mode one of the XM_MD_T values
     * @return true if successful, false otherwise
     */
    bool setMagnetometerMode(XM_MD_T mode);

    /**
     * enable or disable magnetometer low power mode (LPM).  When in
     * low power mode, the magnetometer updates at 3.125Hz, regardless
     * of it's ODR setting.
     *
     * @param enable true to enable LPM, false otherwise
     * @return true if successful, false otherwise
     */
    bool setMagnetometerLPM(bool enable);

    /**
     * set the scaling mode of the magnetometer
     *
     * @param scale one of the XM_MFS_T values
     * @return true if successful, false otherwise
     */
    bool setMagnetometerScale(XM_MFS_T scale);

    /**
     * get the accelerometer values in gravities
     *
     * @param x the returned x value, if arg is non-NULL
     * @param y the returned y value, if arg is non-NULL
     * @param z the returned z value, if arg is non-NULL
     * @return true if successful, false otherwise
     */
    void getAccelerometer(float *x, float *y, float *z);

    /**
     * get the gyroscope values in degrees per second
     *
     * @param x the returned x value, if arg is non-NULL
     * @param y the returned y value, if arg is non-NULL
     * @param z the returned z value, if arg is non-NULL
     * @return true if successful, false otherwise
     */
    void getGyroscope(float *x, float *y, float *z);

    /**
     * get the magnetometer values in gauss
     *
     * @param x the returned x value, if arg is non-NULL
     * @param y the returned y value, if arg is non-NULL
     * @param z the returned z value, if arg is non-NULL
     * @return true if successful, false otherwise
     */
    void getMagnetometer(float *x, float *y, float *z);

#if defined(SWIGJAVA) || defined(JAVACALLBACK)
    /**
     * get the accelerometer values in gravities
     *
     * @return Array containing X, Y, Z acceleration values
     */
    float *getAccelerometer();

    /**
     * get the gyroscope values in degrees per second
     *
     * @return Array containing X, Y, Z gyroscope values
     */
    float *getGyroscope();

    /**
     * get the magnetometer values in gauss
     *
     * @return Array containing X, Y, Z magnetometer values
     */
    float *getMagnetometer();
#endif

    /**
     * get the temperature value.  Unfortunately the datasheet does
     * not provide a mechanism to convert the temperature value into
     * the correct value, so I made a 'guess'.  If it's wrong, and you
     * figure it out, send a patch!
     *
     * @return the temperature value in degrees Celcius
     */
    float getTemperature();

    /**
     * enable onboard temperature measurement sensor
     *
     * @param enable true to enable temperature sensor, false to disable
     * @return true if successful, false otherwise
     */
    bool enableTemperatureSensor(bool enable);

    /**
     * return the gyroscope status register
     *
     * @return bitmask of STATUS_REG_G_BITS_T bits
     */
    uint8_t getGyroscopeStatus();

    /**
     * return the magnetometer status register
     *
     * @return bitmask of STATUS_REG_M_BITS_T bits
     */
    uint8_t getMagnetometerStatus();

    /**
     * return the accelerometer status register
     *
     * @return bitmask of STATUS_REG_A_BITS_T bits
     */
    uint8_t getAccelerometerStatus();

    /**
     * return the gyroscope interrupt config register
     *
     * @return bitmask of INT1_CFG_G_BITS_T bits
     */
    uint8_t getGyroscopeInterruptConfig();

    /**
     * set the gyroscope interrupt config register
     *
     * @param enables bitmask of INT1_CFG_G_BITS_T values
     * @return true if successful
     */
    bool setGyroscopeInterruptConfig(uint8_t enables);

    /**
     * return the gyroscope interrupt src register
     *
     * @return bitmask of INT1_SRC_G_BITS_T bits
     */
    uint8_t getGyroscopeInterruptSrc();

    /**
     * return the magnetometer interrupt control register
     *
     * @return bitmask of INT_CTRL_REG_M_BITS_T bits
     */
    uint8_t getMagnetometerInterruptControl();

    /**
     * set the magnetometer interrupt control register
     *
     * @param enables bitmask of INT_CTRL_REG_M_BITS_T values
     * @return true if successful
     */
    bool setMagnetometerInterruptControl(uint8_t enables);

    /**
     * return the magnetometer interrupt src register
     *
     * @return bitmask of INT_SRC_REG_M_BITS_T bits
     */
    uint8_t getMagnetometerInterruptSrc();

    /**
     * return the inertial interrupt generator 1 register
     *
     * @return bitmask of INT_GEN_X_REG_BITS_T bits
     */
    uint8_t getInterruptGen1();

    /**
     * set the inertial interrupt generator 1 register
     *
     * @param enables bitmask of INT_GEN_X_REG_BITS_T values
     * @return true if successful
     */
    bool setInterruptGen1(uint8_t enables);

    /**
     * return the inertial interrupt generator 1 src register
     *
     * @return bitmask of INT_GEN_X_SRC_BITS_T bits
     */
    uint8_t getInterruptGen1Src();

    /**
     * return the inertial interrupt generator 2 register
     *
     * @return bitmask of INT_GEN_X_REG_BITS_T bits
     */
    uint8_t getInterruptGen2();

    /**
     * set the inertial interrupt generator 2 register
     *
     * @param enables bitmask of INT_GEN_X_REG_BITS_T values
     * @return true if successful
     */
    bool setInterruptGen2(uint8_t enables);

    /**
     * return the inertial interrupt generator 2 src register
     *
     * @return bitmask of INT_GEN_X_SRC_BITS_T bits
     */
    uint8_t getInterruptGen2Src();

#if defined(SWIGJAVA) || defined(JAVACALLBACK)
    void installISR(INTERRUPT_PINS_T intr, int gpio, mraa::Edge level,
		    IsrCallback *cb);
#else
    /**
     * install an interrupt handler.
     *
     * @param intr one of the INTERRUPT_PINS_T values specifying which
     * interrupt pin out of 4 you are installing
     * @param gpio gpio pin to use as interrupt pin
     * @param level the interrupt trigger level (one of mraa::Edge
     * values).  Make sure that you have configured the interrupt pin
     * properly for whatever level you choose.
     * @param isr the interrupt handler, accepting a void * argument
     * @param arg the argument to pass the the interrupt handler
     */
    void installISR(INTERRUPT_PINS_T intr, int gpio, mraa::Edge level, 
                    void (*isr)(void *), void *arg);
#endif
    /**
     * uninstall a previously installed interrupt handler
     *
     * @param intr one of the INTERRUPT_PINS_T values specifying which
     * interrupt pin out of 4 you are uninstalling
     */
    void uninstallISR(INTERRUPT_PINS_T intr);

  protected:
    // uncompensated accelerometer and gyroscope values
    float m_accelX;
    float m_accelY;
    float m_accelZ;

    float m_gyroX;
    float m_gyroY;
    float m_gyroZ;

    float m_magX;
    float m_magY;
    float m_magZ;

    // uncompensated temperature value
    float m_temp;

    // accelerometer and gyro scaling factors, depending on their Full
    // Scale settings.
    float m_accelScale;
    float m_gyroScale;
    float m_magScale;

  private:
    // OR'd with a register, this enables register autoincrement mode,
    // which we need.
#if defined(SWIGJAVA) || defined(JAVACALLBACK)
    void installISR(INTERRUPT_PINS_T intr, int gpio, mraa::Edge level,
                    void (*isr)(void *), void *arg);
#endif

    static const uint8_t m_autoIncrementMode = 0x80;

    mraa::I2c m_i2cG;
    mraa::I2c m_i2cXM;
    uint8_t m_gAddr;
    uint8_t m_xmAddr;

    // return a reference to a gpio pin pointer depending on intr
    mraa::Gpio*& getPin(INTERRUPT_PINS_T intr);

    // possible interrupt pins
    mraa::Gpio *m_gpioG_INT;
    mraa::Gpio *m_gpioG_DRDY;
    mraa::Gpio *m_gpioXM_GEN1;
    mraa::Gpio *m_gpioXM_GEN2;
  };
}


