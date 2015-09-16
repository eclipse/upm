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

#define H3LIS331DL_I2C_BUS 0
#define H3LIS331DL_DEFAULT_I2C_ADDR 0x18

namespace upm {
  
  /**
   * @brief H3LIS331DL I2C Accelerometer (400g) library
   * @defgroup h3lis331dl libupm-h3lis331dl
   * @ingroup seeed i2c accelerometer
   */

  /**
   * @library h3lis331dl
   * @sensor h3lis331dl
   * @comname H3LIS331DL 3-Axis Digital Accelerometer
   * @altname Grove 3-Axis Digital Accelerometer (400g)
   * @type accelerometer
   * @man seeed
   * @web http://www.seeedstudio.com/depot/Grove-3Axis-Digital-Accelerometer400g-p-1897.html
   * @con i2c
   *
   * @brief API for the H3LIS331DL-based Grove 3-Axis Digital Accelerometer (400g)
   *
   * This is a high-performance, high-range accelerometer for extreme applications.
   *
   * @image html h3lis331dl.jpg
   * @snippet h3lis331dl.cxx Interesting
   */
  class H3LIS331DL {
  public:

    /**
     * H3LIS331DL registers
     */
    typedef enum {
      // Reserved bytes must not be written into as they contain
      // factory calibration data. Changing those values may lead to
      // improper functioning of the device.

      // 0x00-0x0E reserved 

      REG_WHOAMI                = 0x0f,

      // 0x10-0x1f reserved

      REG_REG1                  = 0x20,
      REG_REG2                  = 0x21,
      REG_REG3                  = 0x22,
      REG_REG4                  = 0x23,
      REG_REG5                  = 0x24,

      REG_HP_FILTER_RESET       = 0x25,
      REG_REFERENCE             = 0x26,

      REG_STATUS                = 0x27,

      REG_OUT_X_L               = 0x28,
      REG_OUT_X_H               = 0x29,
      REG_OUT_Y_L               = 0x2a,
      REG_OUT_Y_H               = 0x2b,
      REG_OUT_Z_L               = 0x2c,
      REG_OUT_Z_H               = 0x2d,

      // 0x2e, 0x2f reserved

      REG_INT1_CFG              = 0x30,
      REG_INT1_SRC              = 0x31,
      REG_INT1_THS              = 0x32,
      REG_INT1_DUR              = 0x33,

      REG_INT2_CFG              = 0x34,
      REG_INT2_SRC              = 0x35,
      REG_INT2_THS              = 0x36,
      REG_INT2_DUR              = 0x37,

      // 0x38-0x3f reserved
    } H3LIS331DL_REG_T;
    
    /**
     * REG1 bits
     */
    typedef enum {
      REG1_XEN                  = 0x01, // X-axis enable
      REG1_YEN                  = 0x02,
      REG1_ZEN                  = 0x04,

      REG1_DR0                  = 0x08, // data rate
      REG1_DR1                  = 0x10,
      REG1_DR_SHIFT             = 3,    // DR shift

      REG1_PM0                  = 0x20, // power mode
      REG1_PM1                  = 0x40,
      REG1_PM2                  = 0x80,
      REG1_PM_SHIFT             = 5
    } REG1_BITS_T;

    /**
     * REG1 DR (output rate) bits
     */
    typedef enum {
      DR_50_37                  = 0x0, // 50Hz output with 37Hz LPF cutoff
      DR_100_74                 = 0x1,
      DR_400_292                = 0x2,
      DR_1000_780               = 0x3
    } DR_BITS_T;

    /**
     * REG1 PM (power mode) bits
     */
    typedef enum {
      PM_POWERDWN               = 0x0,
      PM_NORMAL                 = 0x1,
      PM_LP05                   = 0x2, // .5 updates/sec
      PM_LP1                    = 0x3, // 1 update/sec
      PM_LP2                    = 0x4,
      PM_LP5                    = 0x5,
      PM_LP10                   = 0x6
    } PM_BITS_T;

    /**
     * REG2 bits
     */
    typedef enum {
      REG2_HPCF0                = 0x01,
      REG2_HPCF1                = 0x02,
      REG2_HPCF_SHIFT           = 0,

      REG2_HPEN1                = 0x04,
      REG2_HPEN2                = 0x08,
      REG2_FDS                  = 0x10,

      REG2_HPM0                 = 0x20,
      REG2_HPM1                 = 0x40,
      REG2_HPM_SHIFT            = 5,

      REG2_BOOT                 = 0x80
    } REG2_BITS_T;

    /**
     * REG2 HPCF (high-pass cutoff frequency) bits
     */
    typedef enum {
      HPCF_8                    = 0x0,
      HPCF_16                   = 0x1,
      HPCF_32                   = 0x2,
      HPCF_64                   = 0x3,
    } HPCF_BITS_T;

    /**
     * REG2 HPM (high-pass filter mode) bits
     */
    typedef enum {
      HPM_NORMAL0               = 0x0,
      HPM_REF                   = 0x1,
      HPM_NORMAL1               = 0x2
    } HPM_BITS_T;

    /**
     * REG3 bits
     */
    typedef enum {
      REG3_I1_CFG0              = 0x01,
      REG3_I1_CFG1              = 0x02,
      REG3_I1_CFG_SHIFT         = 0,

      REG3_LIR1                 = 0x04,

      REG3_I2_CFG0              = 0x08,
      REG3_I2_CFG1              = 0x10,
      REG3_I2_CFG_SHIFT         = 3,

      REG3_LIR2                 = 0x20,
      REG3_PP_OD                = 0x40,
      REG3_IHL                  = 0x80
    } REG3_BITS_T;

    /**
     * REG3 I1/I2 PAD control bits
     */
    typedef enum {
      I_SRC                     = 0x0, // INT source
      I_OR                      = 0x1, // INT1 OR INT2 source
      I_DR                      = 0x2, // Data Ready
      I_BOOTING                 = 0x3  // Boot is running
    } I_CFG_BITS_T;
    
    /**
     * REG4 bits
     */
    typedef enum {
      REG4_SIM                  = 0x01, // SPI 4 or 3 wire

      // bits 01,02,04 reserved

      REG4_FS0                  = 0x10,
      REG4_FS1                  = 0x20,
      REG4_FS_SHIFT             = 4,

      REG4_BLE                  = 0x40, // big/little-endian
      REG4_BDU                  = 0x80  // Block data update
    } REG4_BITS_T;

    /**
     * REG4 FS (full scale) bits
     */
    typedef enum {
      FS_100                    = 0x0, // 100g scale
      FS_200                    = 0x1, // 200g scale
      FS_400                    = 0x3  // 400g scale
    } FS_BITS_T;

    /**
     * REG5 TURNON (sleep to wake) bits
     */
    typedef enum {
      REG5_TURNON0              = 0x01, // turn-on mode for sleep-to-wake
      REG5_TURNON1              = 0x02
 
      // bits 04-80 reserved
    } REG5_BITS_T;

    /**
     * STATUS bits
     */
    typedef enum {
      STATUS_XDA                = 0x01, // X data available
      STATUS_YDA                = 0x02,
      STATUS_ZDA                = 0x04,
      STATUS_ZYXDA              = 0x08, // X, Y, and Z data available
      STATUS_XOR                = 0x10, // X overrun
      STATUS_YOR                = 0x20,
      STATUS_ZOR                = 0x40,
      STATUS_ZYXOR              = 0x80  // X, Y, and Z data overrun
    } STATUS_BITS_T;

    /**
     * INT1/INT2 CFG bits
     */
    typedef enum {
      INT_CFG_XLIE              = 0x01, // enable intr on low X event
      INT_CFG_XHIE              = 0x02, // enable intr on high X event
      INT_CFG_YLIE              = 0x04,
      INT_CFG_YHIE              = 0x08,
      INT_CFG_ZLIE              = 0x10,
      INT_CFG_ZHIE              = 0x20,
      // 0x40 reserved
      INT_CFG_AOI               = 0x80 // AND or OR combination or intrs
    } INT_CFG_BITS_T;

    /**
     * INT1/INT2 SRC bits
     */
    typedef enum {
      INT_SRC_XL                = 0x01, // X low intr event
      INT_SRC_XH                = 0x02, // X high intr event
      INT_SRC_YL                = 0x04,
      INT_SRC_YH                = 0x08,
      INT_SRC_ZL                = 0x10,
      INT_SRC_ZH                = 0x20,
      INT_SRC_IA                = 0x40  // Interrupt generated (active)
      // 0x80 reserved
    } INT_SRC_BITS_T;
    
    /**
     * H3LIS331DL constructor
     *
     * @param bus I2C bus to use
     * @param address Address for this device
     */
    H3LIS331DL(int bus, uint8_t address = H3LIS331DL_DEFAULT_I2C_ADDR);

    /**
     * H3LIS331DL destructor
     */
    ~H3LIS331DL();
    
    /**
     * Sets up initial values and starts operation
     *
     * @param odr Data rate: one of the DR_BITS_T values
     * @param pm Power mode: one of the PM_BITS_T values
     * @param fs FullScale: one of the FS_BITS_T values
     * @return True if successful
     */
    bool init(DR_BITS_T odr=DR_50_37, PM_BITS_T pm=PM_NORMAL, 
              FS_BITS_T fs=FS_100);

    /**
     * Reads and returns the chip ID (WHO_AM_I register)
     *
     * @return True if successful
     */
    uint8_t getChipID();

    /**
     * Sets the output data rate
     *
     * @param One of the DR_BITS_T values
     * @return True if successful
     */
    bool setDataRate(DR_BITS_T odr);

    /**
     * Sets the power mode
     *
     * @param One of the PM_BITS_T values
     * @return True if successful
     */
    bool setPowerMode(PM_BITS_T pm);

    /**
     * Enables one or more of the 3 axes. The argument is a bitmask
     * composed of REG1_XEN, REG1_YEN, and/or REG1_ZEN corresponding to
     * the axes you want enabled.
     *
     * @param axisEnable Bitmask of axes to enable 
     * (REG1_XEN | REG1_YEN | REG1_ZEN)
     * @return True if successful
     */
    bool enableAxis(uint8_t axisEnable);

    /**
     * Sets the scaling factor to 100g, 200g, or 400g
     *
     * @param fs One of the FS_BITS_T values
     * @return True if successful
     */
    bool setFullScale(FS_BITS_T fs);

    /**
     * Sets a high-pass cutoff filter
     *
     * @param val One of the HPCF_BITS_T values
     * @return True if successful
     */
    bool setHPCF(HPCF_BITS_T val);

    /**
     * Sets a high-pass filter mode
     *
     * @param val One of the HPM_BITS_T values
     * @return True if successful
     */
    bool setHPM(HPM_BITS_T val);

    /**
     * Boots the device. Booting the device causes internal flash
     * calibration values to be reloaded into the visible registers
     * in case they have been corrupted. This function
     * returns when the booting is complete.
     *
     * @return True if successful
     */
    bool boot();

    /**
     * Enables a high-pass filter for interrupt 1 source
     *
     * @param enable True to enable the filter, false otherwise
     * @return True if successful
     */
    bool enableHPF1(bool enable);

    /**
     * Enables a high-pass filter for interrupt 2 source
     *
     * @param enable True to enable the filter, false otherwise
     * @return True if successful
     */
    bool enableHPF2(bool enable);

    /**
     * Enables filtered data selection
     *
     * @param enable True to enable, false otherwise
     * @return True if successful
     */
    bool enableFDS(bool enable);

    /**
     * Sets interrupts to be active low instead of high
     *
     * @param enable True to enable, false otherwise
     * @return True if successful
     */
    bool setInterruptActiveLow(bool enable);

    /**
     * Sets an interrupt output mode to open drain rather than push/pull
     *
     * @param enable True to enable, false otherwise
     * @return True if successful
     */
    bool setInterruptOpenDrain(bool enable);

    /**
     * Enables interrupt 1 latch
     *
     * @param enable True to enable, false otherwise
     * @return True if successful
     */
    bool setInterrupt1Latch(bool enable);

    /**
     * Enables interrupt 2 latch
     *
     * @param enable True to enable, false otherwise
     * @return True if successful
     */
    bool setInterrupt2Latch(bool enable);

    /**
     * Sets the interrupt 1 pad configuration
     *
     * @param val One fo the I_CFG_BITS_T values
     * @return True if successful
     */
    bool setInterrupt1PadConfig(I_CFG_BITS_T val);

    /**
     * Sets the interrupt 2 pad configuration
     *
     * @param val One fo the I_CFG_BITS_T values
     * @return True if successful
     */
    bool setInterrupt2PadConfig(I_CFG_BITS_T val);
    
    /**
     * Enables block data update. When enabled, low/high output
     * registers are not updated until both low and high values have
     * been read.
     *
     * @param enable True to enable, false otherwise
     * @return True if successful
     */
    bool enableBDU(bool enable);

    /**
     * Enables big-endian output for 16b reads
     *
     * @param enable True to enable, false otherwise
     * @return True if successful
     */
    bool enableBLE(bool enable);

    /**
     * Enables sleep-to-wake functionality  
     *
     * @param enable True to enable, false otherwise
     * @return True if successful
     */
    bool enableSleepToWake(bool enable);

    /**
     * Returns the contents of the REG_STATUS register
     *
     * @return Contents of the REG_STATUS register
     */
    uint8_t getStatus();

    /**
     * Sets up the interrupt 1 config register
     *
     * @param val Bitmask of desired INT_CFG_BITS_T bits
     * @return True if successful
     */
    bool setInterrupt1Config(uint8_t val);

    /**
     * Sets up the interrupt 2 config register
     *
     * @param val Bitmask of desired INT_CFG_BITS_T bits
     * @return True if successful
     */
    bool setInterrupt2Config(uint8_t val);

    /**
     * Sets up the interrupt 1 source register
     *
     * @param val Bitmask of desired INT_SRC_BITS_T bits
     * @return True if successful
     */
    bool setInterrupt1Source(uint8_t val);

    /**
     * Sets up the interrupt 2 source register
     *
     * @param val Bitmask of desired INT_SRC_BITS_T bits
     * @return True if successful
     */
    bool setInterrupt2Source(uint8_t val);

    /**
     * Sets up the interrupt 1 threshold register
     *
     * @param val Threshhold to set
     * @return True if successful
     */
    bool setInterrupt1Threshold(uint8_t val);

    /**
     * Sets up the interrupt 2 threshold register
     *
     * @param val Threshhold to set
     * @return True if successful
     */
    bool setInterrupt2Threshold(uint8_t val);

    /**
     * Sets up the interrupt 1 duration register
     *
     * @param val Duration to set
     * @return True if successful
     */
    bool setInterrupt1Duration(uint8_t val);

    /**
     * Sets up the interrupt 2 duration register
     *
     * @param val Duration to set
     * @return True if successful
     */
    bool setInterrupt2Duration(uint8_t val);

    /**
     * Reads the sensor and stores current values internally
     */
    void update();

    /**
     * Sets adjustment offsets for each of the axes. This can be used
     * for calibration. The values supplied here are subtracted
     * from the axis data read from the device.
     *
     * @param adjX Amount by which to correct the X-axis measurement
     * @param adjY Amount by which to correct the Y-axis measurement
     * @param adjZ Amount by which to correct the Z-axis measurement
     */
    void setAdjustmentOffsets(int adjX, int adjY, int adjZ);

    /**
     * Gets acceleration values for each of the axes
     *
     * @param aX Returned X-axis acceleration
     * @param aY Returned Y-axis acceleration
     * @param aZ Returned Z-axis acceleration
     */
    void getAcceleration(float *aX, float *aY, float *aZ);

    /**
     * Gets raw axis values
     *
     * @param x Returned raw X-axis value
     * @param y Returned raw Y-axis value
     * @param z Returned raw Z-axis value
     */
    void getRawXYZ(int *x, int *y, int *z);

    /**
     * Gets adjusted axis values
     *
     * @param x Returned X-axis value
     * @param y Returned Y-axis value
     * @param z Returned Z-axis value
     */
    void getXYZ(int *x, int *y, int *z);

#ifdef SWIGJAVA
    /**
     * Gets acceleration values for each of the axes
     *
     * @return Array containing X, Y, Z acceleration values
     */
    float *getAcceleration();

    /**
     * Gets raw axis values
     *
     * @return Array containing X, Y, Z raw values
     */
    int *getRawXYZ();

    /**
     * Gets adjusted axis values
     *
     * @return Array containing X, Y, Z adjusted axis values
     */
    int *getXYZ();
#endif


    /**
     * Provides public access to the MRAA I2C context of the class for
     * direct user access
     *
     * @return Reference to the class I2C context
     */
    mraa::I2c& i2cContext() { return m_i2c; };


  protected:
    int16_t m_rawX, m_rawY, m_rawZ;
    int16_t m_adjX, m_adjY, m_adjZ;
    mraa::I2c m_i2c;

  private:
    uint8_t m_addr;
  };
}


