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

#define AK8975_I2C_BUS 0
#define AK8975_DEFAULT_I2C_ADDR 0x0c

namespace upm {
  
  /**
   * @library mpu9150
   * @sensor ak8975
   * @comname AK8975 3-axis Magnetometer
   * @altname AK9875
   * @type compass
   * @man grove
   * @con i2c
   *
   * @brief API for the AK8975 magnetometer
   *
   * This is a 3-axis magnetometer, which can be used alone, or
   * coupled with another device (such as the mcu9150 9-axis motion
   * sensor). 
   *
   * @snippet ak8975.cxx Interesting
   */
  class AK8975 {
  public:

    /**
     * AK8975 registers
     */
    typedef enum {
      REG_WIA                   = 0x00, // device id

      REG_INFO                  = 0x01, // undocumented (AK proprietary data)

      REG_ST1                   = 0x02, // status 1

      REG_HXL                   = 0x03, // magnetometer data, X axis low byte
      REG_HXH                   = 0x04, // magnetometer data, X axis high byte
      REG_HYL                   = 0x05,
      REG_HYH                   = 0x06,
      REG_HZL                   = 0x07,
      REG_HZH                   = 0x08,

      REG_ST2                   = 0x09, // status 2

      REG_CNTL                  = 0x0a, // control

      // REG_RSV 0x0b reserved

      REG_ASTC                  = 0x0c, // self test (internal mag field)

      // REG_TS1, REG_TS2 0x0d, 0x0e reserved/factory test

      // REG_I2CDIS 0x0f, I2C disable.  Not a good idea to use or support.
      //  write a 0x1b to disable i2c.  This requires a power cycle to undo.

      // These registers hold factory calibrated co-efficients needed to
      // properly compensate for production variations.  They can only be
      // read when device is in fuse mode.  They are used to adjust the
      // measured mag field values.
      REG_ASAX                  = 0x10, // X calibration
      REG_ASAY                  = 0x11,
      REG_ASAZ                  = 0x12
    } AK8975_REG_T;
    
    /**
     * ST1 bits
     */
    typedef enum {
      ST1_DRDY                  = 0x01 // data ready bit
    } ST1_BITS_T;
    
    /**
     * ST2 bits
     */
    typedef enum {
      ST2_DERR                  = 0x04, // data error
      ST2_HOFL                  = 0x08  // measurement overflow
    } ST2_BITS_T;
    
    /**
     * CNTL register, operating mode values
     */
    typedef enum {
      CNTL_PWRDWN               = 0x00, // power down
      CNTL_MEASURE              = 0x01, // single measurement
      CNTL_SELFTEST             = 0x08,
      CNTL_FUSE_ACCESS          = 0x0f  // access fuse (coeff) registers
    } CNTL_MODES_T;
    
    /**
     * ASTC (self test control) bits
     */
    typedef enum {
      ASTC_SELF                 = 0x40 // enable self test
    } ASTC_BITS_T;

    /**
     * ak8975 constructor
     *
     * @param bus i2c bus to use
     * @param address the address for this device
     */
    AK8975(int bus=AK8975_I2C_BUS, uint8_t address=AK8975_DEFAULT_I2C_ADDR);

    /**
     * AK8975 Destructor
     */
    ~AK8975();
    
    /**
     * set up initial values and start operation
     *
     * @param dsr the data sampling rate: one of the DSR_BITS_T values
     * @return true if successful
     */
    bool init();

    /**
     * put the chip into a specific mode
     *
     * @param mode one of the CNTL_MODES_T values
     * @return true if successful
     */
    bool setMode(CNTL_MODES_T mode);

    /**
     * check to see if the ST1_DRDY bit is set, indicating the device
     * can accept commands
     *
     * @return true if device is ready, false otherwise
     */
    bool isReady();

    /**
     * check to see if device is ready and sleep/retry if not.
     * Returns once device indicates it's ready.
     *
     * @return true if device is ready, false if retries exhausted
     */
    bool waitforDeviceReady();

    /**
     * take a measurement
     *
     * @param selfTest true if we are running a self test, false
     * (default) otherwise.
     * @return true if successful, false otherwise
     */
    bool update(bool selfTest=false);

    /**
     * do a self test sequence.  When self test is executed, the
     * device activates internal calibrated magnets, and measures
     * them, updating the measurement registers.  Once complete, the
     * data can be read as usual (getMagnetometer()) and the returned
     * values compared against the following limits to determine
     * correctness:
     *
     * -100 < X < +100; -100 < Y < +100; -1000 < Z < -300
     *
     * @return true if successful, false otherwise
     */
    bool selfTest();

    /**
     * return the compensated values for the x, y, and z axes.  The
     * unit of measurement is in micro-teslas (uT).
     *
     * @param x pointer to returned X axis value
     * @param y pointer to returned Y axis value
     * @param z pointer to returned Z axis value
     */
    void getMagnetometer(float *x, float *y, float *z);


  protected:
    /**
     * compute a compensated magnetometer axis value, based on the raw
     * axis value and a per-device, per-axis adjustment coefficient
     * that was read and stored at init() time.
     *
     * @param value the raw axis value to compensate
     * @param adj the adjustment coefficient 
     * @return true if successful
     */
    float adjustValue(float value, float adj);

    // compensation coefficients (factory set) for this device
    float m_xCoeff;
    float m_yCoeff;
    float m_zCoeff;

    // uncompensated magnetometer readings
    float m_xData;
    float m_yData;
    float m_zData;

  private:
    mraa::I2c m_i2c;
    uint8_t m_addr;

  };
}


