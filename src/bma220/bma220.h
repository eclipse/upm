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

#define BMA220_I2C_BUS 0
#define BMA220_DEFAULT_ADDR 0x0a

namespace upm {
  
  /**
   * @brief BMA220 Accelerometer library
   * @defgroup bma220 libupm-bma220
   * @ingroup i2c gpio accelerometer
   */

  /**
   * @library bma220
   * @sensor bma220
   * @comname BMA220 3-axis Accelerometer
   * @type accelerometer
   * @man dfrobot
   * @con i2c gpio
   * @web http://www.dfrobot.com/index.php?route=product/product&product_id=1085
   *
   * @brief API for the BMA220 3-axis  Accelerometer
   *
   * The BMA220 is a low cost, very small 3-axis accelerometer with 6
   * bits of resolution.  It can also detect orientation and tap events.
   *
   * The BMA220 has a linear acceleration full scale of
   * 2g/4g/8g/16g.
   *
   * While not all of the functionality of this device is supported
   * initially, methods and register definitions are provided that
   * should allow an end user to implement whatever features are
   * required.
   *
   * This driver was developed on a DFRobot BMA (Tiny).
   *
   * @snippet bma220.cxx Interesting
   */

  class BMA220 {
  public:

    // NOTE: reserved registers must not be written into or read from.
    // Reserved bitfields must always be 0.  The registers aren't
    // named in the datasheet, so I made up some hopefully useful
    // names for them.  The whole I2C register map design is a little
    // strange, IMO.

    /**
     * BMA220 Accelerometer registers
     */
    typedef enum {
      REG_CHIPID                = 0x00,
      REG_REVISIONID            = 0x02,

      // 2 lsb bits of ACC regs are always 0, yeilding 6 bits resolution
      REG_ACC_X                 = 0x04, // acceleration data
      REG_ACC_Y                 = 0x06,
      REG_ACC_Z                 = 0x08,

      REG_H_HYST_DUR            = 0x0a, // high hysteresis/dur
      REG_THRESHOLD             = 0x0c, // high/low threshold
      REG_L_HYST_DUR            = 0x0e, // low hysteresis/dur

      REG_TAP_CONFIG            = 0x10,

      REG_SLOPE_CONFIG          = 0x12,
      REG_TAP_CONFIG2           = 0x14,

      REG_INT_STATUS1           = 0x16,
      REG_INT_STATUS2           = 0x18,

      REG_ENABLE_CONFIG         = 0x1a,
      REG_ENABLE_CONFIG2        = 0x1c,
      REG_ENABLE_CONFIG3        = 0x1e,

      REG_FILTER_CONFIG         = 0x20,

      REG_SBIST_FSL_CONFIG      = 0x22, // self test and full scale range

      // 0x24 - 0x2c reserved

      REG_I2C_WATCHDOG          = 0x2e, // + SPI 3-wire mode

      REG_SUSPEND               = 0x30,
      REG_SOFTRESET             = 0x32
    } REG_T;
    

    /**
     * REG_H_HYST_DUR bits
     */
    typedef enum {
      H_HYST_DUR_HIGH_DUR0         = 0x01,
      H_HYST_DUR_HIGH_DUR1         = 0x02,
      H_HYST_DUR_HIGH_DUR2         = 0x04,
      H_HYST_DUR_HIGH_DUR3         = 0x08,
      H_HYST_DUR_HIGH_DUR4         = 0x10,
      H_HYST_DUR_HIGH_DUR5         = 0x20,
      _H_HYST_DUR_HIGH_DUR_MASK    = 63,
      _H_HYST_DUR_HIGH_DUR_SHIFT   = 0,

      H_HYST_DUR_HIGH_HY1          = 0x40,
      H_HYST_DUR_HIGH_HY2          = 0x80,
      _H_HYST_DUR_HIGH_HY_MASK     = 3,
      _H_HYST_DUR_HIGH_HY_SHIFT    = 6
    } H_HYST_DUR_HIGH_BITS_T;

    /**
     * REG_THRESHOLD bits
     */
    typedef enum {
      THRESHOLD_HIGH0              = 0x01,
      THRESHOLD_HIGH1              = 0x02,
      THRESHOLD_HIGH2              = 0x04,
      THRESHOLD_HIGH3              = 0x08,
      _THRESHOLD_HIGH_MASK         = 15,
      _THRESHOLD_HIGH_SHIFT        = 0,

      THRESHOLD_LOW0               = 0x10,
      THRESHOLD_LOW1               = 0x20,
      THRESHOLD_LOW2               = 0x40,
      THRESHOLD_LOW3               = 0x80,
      _THRESHOLD_LOW_MASK          = 15,
      _THRESHOLD_LOW_SHIFT         = 4
    } THRESHOLD_BITS_T;

    /**
     * REG_L_HYST_DUR bits
     */
    typedef enum {
      L_HYST_DUR_LOW_DUR0          = 0x01,
      L_HYST_DUR_LOW_DUR1          = 0x02,
      L_HYST_DUR_LOW_DUR2          = 0x04,
      L_HYST_DUR_LOW_DUR3          = 0x08,
      L_HYST_DUR_LOW_DUR4          = 0x10,
      L_HYST_DUR_LOW_DUR5          = 0x20,
      _L_HYST_DUR_LOW_DUR_MASK     = 63,
      _L_HYST_DUR_LOW_DUR_SHIFT    = 0,

      L_HYST_DUR_LOW_HY1           = 0x40,
      L_HYST_DUR_LOW_HY2           = 0x80,
      _L_HYST_DUR_LOW_HY_MASK      = 3,
      _L_HYST_DUR_LOW_HY_SHIFT     = 6
    } L_HYST_DUR_LOW_BITS_T;

    /**
     * REG_TAP_CONFIG bits
     */
    typedef enum {
      TAP_CONFIG_DUR0              = 0x01,
      TAP_CONFIG_DUR1              = 0x02,
      TAP_CONFIG_DUR2              = 0x04,
      _TAP_CONFIG_DUR_MASK         = 7,
      _TAP_CONFIG_DUR_SHIFT        = 0,

      TAP_CONFIG_THRESH0           = 0x08,
      TAP_CONFIG_THRESH1           = 0x10,
      TAP_CONFIG_THRESH2           = 0x20,
      TAP_CONFIG_THRESH3           = 0x40,
      _TAP_CONFIG_THRESH_MASK      = 15,
      _TAP_CONFIG_THRESH_SHIFT     = 3,

      TAP_CONFIG_FILTER            = 0x80
    } TAP_CONFIG_BITS_T;

    /**
     * REG_SLOPE_CONFIG bits
     */
    typedef enum {
      SLOPE_CONFIG_DUR0            = 0x01,
      SLOPE_CONFIG_DUR1            = 0x02,
      _SLOPE_CONFIG_DUR_MASK       = 3,
      _SLOPE_CONFIG_DUR_SHIFT      = 0,

      SLOPE_CONFIG_THRESH0         = 0x04,
      SLOPE_CONFIG_THRESH1         = 0x08,
      SLOPE_CONFIG_THRESH2         = 0x10,
      SLOPE_CONFIG_THRESH3         = 0x20,
      _SLOPE_CONFIG_THRESH_MASK    = 15,
      _SLOPE_CONFIG_THRESH_SHIFT   = 2,

      SLOPE_CONFIG_FILTER          = 0x40,
      SLOPE_CONFIG_ORIENT_EX       = 0x80 // exchange x and z axis for orient
    } SLOPE_CONFIG_BITS_T;

    /**
     * REG_TAP_CONFIG2 bits
     */
    typedef enum {
      TAP_CONFIG2_SAMP0            = 0x01,
      TAP_CONFIG2_SAMP1            = 0x02,
      _TAP_CONFIG2_SAMP_MASK       = 3,
      _TAP_CONFIG2_SAMP_SHIFT      = 0,

      TAP_CONFIG2_ORIENT_BLOCK0    = 0x04,
      TAP_CONFIG2_ORIENT_BLOCK1    = 0x08,
      _TAP_CONFIG2_ORIENT_BLOCK_MASK = 3,
      _TAP_CONFIG2_ORIENT_BLOCK_SHIFT = 2,

      TAP_CONFIG2_TIP_EN           = 0x10

      // 0x20-0x80 reserved
    } TAP_CONFIG2_BITS_T;

    /**
     * TAP_CONFIG2_SAMP values
     */
    typedef enum {
      TAP_SAMP_2                   = 0, // 2 data samples after wakeup
      TAP_SAMP_4                   = 1,
      TAP_SAMP_8                   = 2,
      TAP_SAMP_16                  = 3
    } TAP_SAMP_T;

    /**
     * TAP_CONFIG2_ORIENT_BLOCK values
     *
     */
    typedef enum {
      TAP_ORIENT_BLOCK_0           = 0, // orient blocking disabled
      TAP_ORIENT_BLOCK_2           = 1, // |z|>0.9g OR |x|+|y| < 0.2g OR m<0.2g
      TAP_ORIENT_BLOCK_3           = 2, // |z|>0.9g OR |x|+|y| < 0.3g OR m<0.3g
      TAP_ORIENT_BLOCK_4           = 3, // |z|>0.9g OR |x|+|y| < 0.4g OR m<0.4g
    } TAP_ORIENT_BLOCK_T;

    /**
     * REG_INT_STATUS1 bits
     */
    typedef enum {
      INT_STATUS1_SIGN              = 0x01,
      INT_STATUS1_FIRST_Z           = 0x02,
      INT_STATUS1_FIRST_Y           = 0x04,
      INT_STATUS1_FIRST_X           = 0x08,

      INT_STATUS1_ORIENT0           = 0x10,
      INT_STATUS1_ORIENT1           = 0x20,
      INT_STATUS1_ORIENT2           = 0x40,
      _INT_STATUS1_ORIENT_MASK      = 7,
      _INT_STATUS1_ORIENT_SHIFT     = 4,

      INT_STATUS1_ORIENT_INT         = 0x80 // orient intr was generated
    } INT_STATUS1_BITS_T;
      
    /**
     * INT_STATUS1_ORIENT values
     *
     * These are values reported by the device if orientation
     * detection is enabled.
     */
    typedef enum {
      CONFIG_ORI_UP_PORT_UPRIGHT     = 0, // up portrait
      CONFIG_ORI_UP_PORT_UPSIDE_DOWN = 1,
      CONFIG_ORI_UP_LAND_LEFT        = 2, // landscape
      CONFIG_ORI_UP_LAND_RIGHT       = 3,
      CONFIG_ORI_DN_PORT_UPRIGHT     = 4, // down portrait
      CONFIG_ORI_DN_PORT_UPSIDE_DOWN = 5,
      CONFIG_ORI_DN_LAND_LEFT        = 6, // landscape
      CONFIG_ORI_DN_LAND_RIGHT       = 7
    } CONFIG_ORIENT_T;

    /**
     * REG_INT_STATUS2 bits
     */
    typedef enum {
      INT_STATUS2_SLOPE            = 0x01,
      INT_STATUS2_DATA             = 0x02,
      INT_STATUS2_HIGH             = 0x04,
      INT_STATUS2_LOW              = 0x08,
      INT_STATUS2_TAP              = 0x10

      // 0x20-0x80 reserved
    } INT_STATUS2_BITS_T;

    /**
     * REG_ENABLE_CONFIG bits
     */
    typedef enum {
      ENABLE_CONFIG_TT_Z           = 0x01,
      ENABLE_CONFIG_TT_Y           = 0x02,
      ENABLE_CONFIG_TT_X           = 0x04,
      ENABLE_CONFIG_SLOPE_Z        = 0x08,
      ENABLE_CONFIG_SLOPE_Y        = 0x10,
      ENABLE_CONFIG_SLOPE_X        = 0x20,
      ENABLE_CONFIG_ORIENT         = 0x40,
      ENABLE_CONFIG_DATA           = 0x80
    } ENABLE_CONFIG_BITS_T;

    /**
     * REG_ENABLE_CONFIG2 bits
     */
    typedef enum {
      ENABLE_CONFIG2_HIGH_Z        = 0x01,
      ENABLE_CONFIG2_HIGH_Y        = 0x02,
      ENABLE_CONFIG2_HIGH_X        = 0x04,

      ENABLE_CONFIG2_LOW           = 0x08,

      ENABLE_CONFIG2_LAT_INT0      = 0x10, // interrupt latching
      ENABLE_CONFIG2_LAT_INT1      = 0x20,
      ENABLE_CONFIG2_LAT_INT2      = 0x40,
      _ENABLE_CONFIG2_LAT_INT_MASK = 7,
      _ENABLE_CONFIG2_LAT_INT_SHIFT = 4,

      ENABLE_CONFIG2_RESET_INT     = 0x80 // reset interrupts
    } ENABLE_CONFIG2_BITS_T;

    /**
     * ENABLE_CONFIG2_LAT values
     *
     * These are values that define the interrupt latching behavior
     */
    typedef enum {
      CONFIG2_LAT_UNLATCH          = 0, // unlatched intrs
      CONFIG2_LAT_0_25             = 1, // latch intr for 0.25s
      CONFIG2_LAT_0_5              = 2, // latch intr for 0.5s
      CONFIG2_LAT_1                = 3, // latch intr for 1s
      CONFIG2_LAT_2                = 4, // latch intr for 2s
      CONFIG2_LAT_4                = 5, // latch intr for 4s
      CONFIG2_LAT_8                = 6, // latch intr for 8s
      CONFIG2_LAT_PERM             = 7  // latch permanently
    } CONFIG2_LAT_T;

    /**
     * REG_ENABLE_CONFIG3 bits
     */
    typedef enum {
      ENABLE_CONFIG3_Z_CHAN        = 0x01,
      ENABLE_CONFIG3_Y_CHAN        = 0x02,
      ENABLE_CONFIG3_X_CHAN        = 0x04,

      ENABLE_CONFIG3_SLEEP_DUR0    = 0x08,
      ENABLE_CONFIG3_SLEEP_DUR1    = 0x10,
      ENABLE_CONFIG3_SLEEP_DUR2    = 0x20,
      _ENABLE_CONFIG3_SLEEP_DUR_MASK = 7,
      _ENABLE_CONFIG3_SLEEP_DUR_SHIFT = 3,

      ENABLE_CONFIG3_SLEEP_EN      = 0x40

      // 0x80 reserved
    } ENABLE_CONFIG3_BITS_T;

    /**
     *  ENABLE_CONFIG3_SLEEP_DUR values
     *
     * These are values that define the length of time the device
     * sleeps before sampling when in sleep mode.
     */
    typedef enum {
      SLEEP_DUR_2MS                = 0, // 2 ms
      SLEEP_DUR_10MS               = 1,
      SLEEP_DUR_25MS               = 2,
      SLEEP_DUR_50MS               = 3,
      SLEEP_DUR_100MS              = 4,
      SLEEP_DUR_500MS              = 5,
      SLEEP_DUR_1S                 = 6, // 1 second
      SLEEP_DUR_2S                 = 7
    } SLEEP_DUR_T;

    /**
     * REG_FILTER_CONFIG bits
     */
    typedef enum {
      FILTER_CONFIG_FILTER0        = 0x01,
      FILTER_CONFIG_FILTER1        = 0x02,
      FILTER_CONFIG_FILTER2        = 0x04,
      FILTER_CONFIG_FILTER3        = 0x08,
      _FILTER_CONFIG_FILTER_MASK   = 15,
      _FILTER_CONFIG_FILTER_SHIFT  = 0,

      // 0x10-0x40 reserved

      FILTER_CONFIG_SERIAL_HIGH_BW = 0x80
    } FILTER_CONFIG_BITS_T;

    /**
     * FILTER_CONFIG_FILTER values
     *
     * These are values that define the digital filtering frequency
     */
    typedef enum {
      FILTER_CONFIG_1KHZ           = 0, // 1Khz
      FILTER_CONFIG_500HZ          = 1,
      FILTER_CONFIG_250HZ          = 2,
      FILTER_CONFIG_125HZ          = 3,
      FILTER_CONFIG_64HZ           = 4,
      FILTER_CONFIG_32HZ           = 5
    } FILTER_CONFIG_T;

    /**
     * REG_SBIST_FSL_CONFIG bits
     */
    typedef enum {
      REG_SBIST_FSL_RANGE0         = 0x01, // full scale range
      REG_SBIST_FSL_RANGE1         = 0x02,
      _REG_SBIST_FSL_RANGE_MASK    = 3,
      _REG_SBIST_FSL_RANGE_SHIFT   = 0,

      REG_SBIST_FSL_SBIST0         = 0x04, // self test enables
      REG_SBIST_FSL_SBIST1         = 0x08,
      _REG_SBIST_FSL_SBIST_MASK    = 3,
      _REG_SBIST_FSL_SBIST_SHIFT   = 2,

      REG_SBIST_FSL_SBIST_SIGN     = 0x10 // signedness of self test

      // 0x20-0x80 reserved
    } SBIST_FSL_CONFIG_BITS_T;

    /**
     * REG_SBIST_FSL_RANGE values
     *
     * These are values that define the Full Scale configuration
     */
    typedef enum {
      FSL_RANGE_2G                 = 0, // 2G FSL
      FSL_RANGE_4G                 = 1,
      FSL_RANGE_8G                 = 2,
      FSL_RANGE_16G                = 3
    } FSL_RANGE_T;

    /**
     * REG_SBIST_FSL_SBIST values
     *
     * These are values that enable self test mode
     */
    typedef enum {
      SBIST_OFF                    = 0, // self test off
      SBIST_X                      = 1, // self test X
      SBIST_Y                      = 2,
      SBIST_Z                      = 3
    } SBIST_T;

    /**
     * REG_I2C_WATCHDOG bits
     */
    typedef enum {
      I2C_WATCHDOG_SPI3            = 0x01, // SPI 3wire mode (SPI not supported)

      I2C_WATCHDOG_TO_SEL          = 0x02,
      I2C_WATCHDOG_TO_EN           = 0x04

      // 0x08-0x80 reserved
    } I2C_WATCHDOG_BITS_T;


    /**
     * BMA220 constructor
     *
     * @param bus i2c bus to use
     * @param address the address for this device
     */
    BMA220(int bus=BMA220_I2C_BUS, uint8_t addr=BMA220_DEFAULT_ADDR);

    /**
     * BMA220 Destructor
     */
    ~BMA220();
    
    /**
     * update the accelerometer values
     */
    void update();

    /**
     * read a register
     *
     * @param dev the device to access (XM or G)
     * @param reg the register to read
     * @return the value of the register
     */
    uint8_t readReg(uint8_t reg);

    /**
     * write to a register
     *
     * @param dev the device to access (XM or G)
     * @param reg the register to write to
     * @param val the value to write
     * @return true if successful, false otherwise
     */
    bool writeReg(uint8_t reg, uint8_t val);

    /**
     * return the chip ID
     *
     * @return the chip ID (usually 0xdd)
     */
    uint8_t getChipID();

    /**
     * return the chip revision
     *
     * @return the chip revision (usually 0x00)
     */
    uint8_t getChipRevision();

    /**
     * set the scaling mode of the accelerometer (2g/4g/8g/16g)
     *
     * @param scale one of the FSL_RANGE_T values
     * @return true if successful, false otherwise
     */
    bool setAccelerometerScale(FSL_RANGE_T scale);

    /**
     * get the accelerometer values in gravities
     *
     * @param x the returned x value, if arg is non-NULL
     * @param y the returned y value, if arg is non-NULL
     * @param z the returned z value, if arg is non-NULL
     */
    void getAccelerometer(float *x, float *y, float *z);

#if defined(SWIGJAVA) || defined(JAVACALLBACK)
    /**
     * get the accelerometer values in gravities
     *
     * @return Array containing X, Y, Z acceleration values
     */
    float *getAccelerometer();
#endif

    /**
     * set the filtering configuration
     *
     * @param filter one of the FILTER_CONFIG_T values
     * @return true if successful
     */
    bool setFilterConfig(FILTER_CONFIG_T filter);

    /**
     * enable or disable high bandwidth serial access (1Khz).  This
     * essentially disables filtering and makes the raw unfiltered
     * data available in the axis registers.
     *
     * @param high true to enable high bw access, false otherwise
     * @return true if successful
     */
    bool setSerialHighBW(bool high);

    /**
     * enable or disable axis channels
     *
     * @param xEn true to enable the axis, false otherwise
     * @param yEn true to enable the axis, false otherwise
     * @param zEn true to enable the axis, false otherwise
     * @return true if successful
     */
    bool enableAxes(bool xEn, bool yEn, bool zEn);

    /**
     * place the device into, or take the device out of suspend mode
     *
     * @return 0x00 if the device was in active mode, 0xff if the
     * device was in suspend mode
     */
    uint8_t suspend();

    /**
     * place the device into, or take the device out of soft reset mode
     *
     * @return 0x00 if the device was in active mode, 0xff if the
     * device was in soft reset mode
     */
    uint8_t softReset();

    /**
     * place the device into, or take the device out of low power
     * mode.  See the datasheet for information on how low power mode
     * is implemented on this device.
     *
     * @param enable true to set low power mode, false otherwise
     * @return true if successful
     */
    bool sleep(bool enable);

    /**
     * when in low power (sleep) mode, specify how often the device
     * wakes up to acquire samples.
     *
     * @param dur one of the SLEEP_DUR_T values
     * @return true if successful
     */
    bool setSleepDuration(SLEEP_DUR_T dur);

    /**
     * specify the threshold for low G detection
     *
     * @param thresh see the datasheet
     * @return true if successful
     */
    bool setLowGThreshold(uint8_t thresh);

    /**
     * specify the threshold for high G detection
     *
     * @param thresh see the datasheet
     * @return true if successful
     */
    bool setHighGThreshold(uint8_t thresh);

    /**
     * specify the hysteresis for low G detection
     *
     * @param hyst 2 bit hysteresis value
     * @return true if successful
     */
    bool setLowGHysteresis(uint8_t hyst);

    /**
     * specify the sample duration for low G detection
     *
     * @param dur the number of samples (depends on bandwidth)
     * @return true if successful
     */
    bool setLowGDuration(uint8_t dur);

    /**
     * specify the hysteresis for high G detection
     *
     * @param hyst 2 bit hysteresis value
     * @return true if successful
     */
    bool setHighGHysteresis(uint8_t hyst);

    /**
     * specify the sample duration for high G detection
     *
     * @param dur the number of samples (depends on bandwidth)
     * @return true if successful
     */
    bool setHighGDuration(uint8_t dur);

    /**
     * specify the sample duration for tap detection
     *
     * @param dur the number of samples (depends on bandwidth)
     * @return true if successful
     */
    bool setTapDuration(uint8_t dur);

    /**
     * specify the threshold for tap detection
     *
     * @param thresh see the datasheet
     * @return true if successful
     */
    bool setTapThreshold(uint8_t thresh);

    /**
     * unable to disable tap filtering
     *
     * @param filt true to enable, false otherwise
     * @return true if successful
     */
    bool enableTapFilter(bool filt);

    /**
     * specify the sample duration for slope detection
     *
     * @param dur the number of samples (depends on bandwidth)
     * @return true if successful
     */
    bool setSlopeDuration(uint8_t dur);

    /**
     * specify the threshold for slope detection
     *
     * @param thresh see the datasheet
     * @return true if successful
     */
    bool setSlopeThreshold(uint8_t thresh);

    /**
     * enable or disable slope filtering
     *
     * @param filt true to enable filtering, false otherwise
     * @return true if successful
     */
    bool enableSlopeFilter(bool filt);

    /**
     * return a bitmask of the interrupt status 1 register
     *
     * @return bitmask of INT_STATUS1_BITS_T bits (minus the orient value)
     */
    uint8_t getInterruptStatus1();

    /**
     * return the orient value from the interrupt status 1 register
     *
     * @return one of the CONFIG_ORIENT_T values
     */
    CONFIG_ORIENT_T getOrient();

    /**
     * return a bitmask of the interrupt status 2 register
     *
     * @return bitmask of INT_STATUS2_BITS_T bits
     */
    uint8_t getInterruptStatus2();

    /**
     * enable interrupts for events in interrupt config register 1
     *
     * @param bits bitmask of ENABLE_CONFIG_BITS_T values
     * @return true if successful
     */
    bool setInterruptEnables1(uint8_t bits);

    /**
     * return the enabled interrupts for events in interrupt config register 1
     *
     * @return bitmask of ENABLE_CONFIG_BITS_T values
     */
    uint8_t getInterruptEnables1();

    /**
     * enable interrupts for events in interrupt config register 2
     *
     * @param bits bitmask of ENABLE_CONFIG2_BITS_T values
     * @return true if successful
     */
    bool setInterruptEnables2(uint8_t bits);

    /**
     * return the enabled interrupts for events in interrupt config register 2
     *
     * @return bitmask of ENABLE_CONFIG2_BITS_T values
     */
    uint8_t getInterruptEnables2();

    /**
     * configure interrupt latching behavior
     *
     * @param lat one of the CONFIG2_LAT_T values
     * @return true if successful
     */
    bool setInterruptLatch(CONFIG2_LAT_T lat);

    /**
     * reset the interrupt controller.  This should be called after
     * any changes are made to interrupt configuration, or to reset
     * interrupts if latched interrupts have been enabled and an
     * interrupt has occurred..
     *
     * @return true if successful
     */
    bool resetInterrupts();

#if defined(SWIGJAVA) || defined(JAVACALLBACK)
    void installISR(int gpio, mraa::Edge level, IsrCallback *cb);
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

    // uncompensated accelerometer values
    float m_accelX;
    float m_accelY;
    float m_accelZ;

    // accelerometer full scale
    float m_accelScale;

  private:
    /**
     * update the accelerometer values
     */
    void updateAccelerometer();
    // Adding a private function definition for java bindings
#if defined(SWIGJAVA) || defined(JAVACALLBACK)
    void installISR(int gpio, mraa::Edge level, 
                    void (*isr)(void *), void *arg);
#endif

  };
}


