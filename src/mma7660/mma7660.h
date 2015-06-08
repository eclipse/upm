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
#include <mraa/i2c.h>
#include <mraa/gpio.h>

#define MMA7660_I2C_BUS 0
#define MMA7660_DEFAULT_I2C_ADDR 0x4c

namespace upm {

  /**
   * @brief MMA7660 I2C 3-axis digital accelerometer library
   * @defgroup mma7660 libupm-mma7660
   * @ingroup seeed i2c gpio accelerometer
   */
  /**
   * @library mma7660
   * @sensor mma7660
   * @comname MMA7660 3-Axis Digital Accelerometer
   * @altname Grove 3-Axis Digital Accelerometer (1.5g)
   * @type accelerometer
   * @man seeed
   * @con i2c gpio
   *
   * @brief API for the MMA7660 I2C 3-axis digital accelerometer
   *
   * UPM module for the MMA7660 I2C 3-axis digital accelerometer.
   * This device supports a variety of capabilities, including the
   * generation of interrupts for various conditions, tilt and basic
   * gesture detection, and of course X/Y/Z measurements of g-forces
   * being applied (up to 1.5g)
   *
   * This module was tested with the Grove 3-Axis Digital
   * Accelerometer (1.5g)
   *
   * @image html mma7660.jpg
   * @snippet mma7660.cxx Interesting
   */
  class MMA7660 {
  public:

    // MMA7660 registers
    typedef enum { REG_XOUT       = 0x00,
                   REG_YOUT       = 0x01,
                   REG_ZOUT       = 0x02,
                   REG_TILT       = 0x03,
                   REG_SRST       = 0x04, // Sampling Rate Status
                   REG_SPCNT      = 0x05, // sleep count
                   REG_INTSU      = 0x06, // Interrupt setup
                   REG_MODE       = 0x07, // operating mode
                   REG_SR         = 0x08, // auto wake/sleep, SPS and debounce
                   REG_PDET       = 0x09, // tap detection
                   REG_PD         = 0x0a  // tap debounce count
                   // 0x0b-0x1f reserved
    } MMA7660_REG_T;
    
    // interrupt enable register bits
    typedef enum { INTR_NONE          = 0x00, // disabled
                   INTR_FBINT         = 0x01, // front/back
                   INTR_PLINT         = 0x02, // up/down/right/left
                   INTR_PDINT         = 0x04, // tap detection
                   INTR_ASINT         = 0x08, // exit autosleep
                   INTR_GINT          = 0x10, // measurement intr
                   INTR_SHINTZ        = 0x20, // shake on Z
                   INTR_SHINTY        = 0x40, // shake on Y
                   INTR_SHINTX        = 0x80 // shake on X
    } MMA7660_INTR_T;
    
    // operating mode register bits
    typedef enum { MODE_MODE          = 0x01, // determines mode with MODE_TON
                   // 0x02 reserved
                   MODE_TON           = 0x04, // determines mode with MODE_MODE
                   MODE_AWE           = 0x08, // auto-wake
                   MODE_ASE           = 0x10, // auto-sleep
                   MODE_SCPS          = 0x20, // sleep count pre-scale
                   MODE_IPP           = 0x40, // intr out push-pull/open drain
                   MODE_IAH           = 0x80  // intr active low/high
    } MMA7660_MODE_T;
    
    // tilt BackFront (BF) bits
    typedef enum { BF_UNKNOWN          = 0x00,
                   BF_LYING_FRONT      = 0x01,
                   BF_LYING_BACK       = 0x02
    } MMA7660_TILT_BF_T;    

    // tilt LandscapePortrait (LP) bits
    typedef enum { LP_UNKNOWN          = 0x00,
                   LP_LANDSCAPE_LEFT   = 0x01,
                   LP_LANDSCAPE_RIGHT  = 0x02,
                   LP_VERT_DOWN        = 0x05,
                   LP_VERT_UP          = 0x06
    } MMA7660_TILT_LP_T;    

    // sample rate (auto sleep) values
    typedef enum { AUTOSLEEP_120   = 0x00,
                   AUTOSLEEP_64    = 0x01,
                   AUTOSLEEP_32    = 0x02,
                   AUTOSLEEP_16    = 0x03,
                   AUTOSLEEP_8     = 0x04,
                   AUTOSLEEP_4     = 0x05,
                   AUTOSLEEP_2     = 0x06,
                   AUTOSLEEP_1     = 0x07
    } MMA7660_AUTOSLEEP_T;    

    /**
     * mma7660 constructor
     *
     * @param bus i2c bus to use
     * @param address the address for this sensor; default is 0x55
     */
    MMA7660(int bus, uint8_t address = MMA7660_DEFAULT_I2C_ADDR);

    /**
     * MMA7660 Destructor
     */
    ~MMA7660();

    /**
     * Write byte value into register
     *
     * @param reg register location to write into
     * @param byte byte to write
     * @return true if successful
     */
    bool writeByte(uint8_t reg, uint8_t byte);

    /**
     * Read byte value from register
     *
     * @param reg register location to read from
     * @return value at specified register
     */
    uint8_t readByte(uint8_t reg);

    /**
     * Read current value of conversion
     *
     * @param x returned x value
     * @param y returned y value
     * @param z returned z value
     */
    void getRawValues(int *x, int *y, int *z);

    /**
     * Get the computed acceleration
     *
     * @param ax returned computed acceleration of X axis
     * @param ay returned computed acceleration of Y axis
     * @param az returned computed acceleration of Z axis
     */
    void getAcceleration(float *ax, float *ay, float *az);

    /**
     * Read an axis, verifying it's validity.  The value passed must
     * be one of REG_XOUT, REG_YOUT, or REG_ZOUT.
     *
     * @param axis axis to read
     * @return axis value
     */
    int getVerifiedAxis(MMA7660_REG_T axis);

    /**
     * Read the tilt register, verifying it's validity.
     *
     * @return tilt value
     */
    uint8_t getVerifiedTilt();

    /**
     * Put the device into active mode.  In active mode, register
     * write are not allowed.  Place the device in Standby mode before
     * attempting to write registers.
     *
     */
    void setModeActive();

    /**
     * Put the device into Standby (power saving) mode.  Note, when in
     * standby mode, there will be no valid data in the registers.  In
     * addition, the only way to write a register is to place the
     * device in standby mode.
     *
     */
    void setModeStandby();

    /**
     * Read tilt BackFront bits
     *
     * The value returned will be one of the MMA7660_TILT_BF_T values
     *
     * @return the bits corresponding to the BackFront tilt status
     */
    uint8_t tiltBackFront();
    
    /**
     * Read tilt LandscapePortrait bits
     *
     * The value returned will be one of the MMA7660_TILT_LP_T values
     *
     * @return the bits corresponding to the LandscapePortrait tilt status
     */
    uint8_t tiltLandscapePortrait();
    
    /**
     * read the tilt Tap status
     *
     * @return true if a tap was detected
     */
    bool tiltTap();
    
    /**
     * read the tilt Shake status
     *
     * @return true if a Shake was detected
     */
    bool tiltShake();
    
    /**
     * Install an Interrupt Service Routine (ISR) to be called when
     * an interrupt occurs
     *
     * @param pin gpio pin to use as interrupt pin
     * @param fptr function pointer to function to be called on interrupt
     * @param arg pointer to an object that will be supplied as an
     * argument to the ISR.
     */
    void installISR(int pin, void (*isr)(void *), void *arg);

    /**
     * Uninstall the previously installed Interrupt Service Routine (ISR)
     *
     */
    void uninstallISR();

    /**
     * Enable interrupt generation based on the passed interrupt bits.
     * The bits are a bit mask of the requested MMA7660_INTR_T values.
     * Note: The device must be in standby mode to set this register.
     *
     * @param ibits set the requested interrupt bits
     * @return true if successful
     */
    bool setInterruptBits(uint8_t ibits);

    /**
     * Set the sampling rate of the sensor.  The value supplied must
     * be one of the MMA7660_AUTOSLEEP_T values.
     *
     * @param sr one of the MMA7660_AUTOSLEEP_T values
     * @return true if successful
     */
    bool setSampleRate(MMA7660_AUTOSLEEP_T sr);

  private:
    bool m_isrInstalled;
    mraa_i2c_context m_i2c;
    mraa_gpio_context m_gpio;
    uint8_t m_addr;
  };
}


