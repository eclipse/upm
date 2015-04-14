/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
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

#define GROVEMD_I2C_BUS 0
#define GROVEMD_DEFAULT_I2C_ADDR 0x0f

// This is a NOOP value used to pad packets
#define GROVEMD_NOOP 0x01

namespace upm {
  /**
   * @brief UPM module for the Grove I2C Motor Driver
   * @defgroup grovemd libupm-grovemd
   * @ingroup seeed i2c motor
   */

  /**
   * @library grovemd
   * @sensor grovemd
   * @comname Grove I2C Motor Driver
   * @type motor
   * @man seeed
   * @con i2c
   *
   * @brief C++ API for the Grove I2C Motor Driver 
   *
   * This class implements support for the Grove I2C Motor Driver.
   * This device can support a single 4-wire stepper motor, OR two
   * 2-wire DC motors.  The device contains an Atmel ATmega8L
   * microcontroller that manages an L298N H-bridge driver chip.
   *
   * This device supports an i2c bus speed of 100Khz only.
   *
   * The module does not provide any telemetry or status -- it only
   * accepts I2C commands for its various operations.
   *
   * This module was tested with version 1.3 of the Grove I2C Motor
   * Driver
   *
   * @snippet grovemd.cxx Interesting
   */
  class GroveMD {

  public:
    // GroveMD registers
    typedef enum { SET_SPEED           = 0x82,
                   SET_PWM_FREQ        = 0x84,
                   SET_DIRECTION       = 0xaa,
                   SET_MOTOR_A         = 0xa1, // not documented
                   SET_MOTOR_B         = 0xa5, // not documented
                   STEPPER_ENABLE      = 0x1a, 
                   STEPPER_DISABLE     = 0x1b,
                   STEPPER_NUM_STEPS   = 0x1c
    } REG_T;

    // legal directions for the stepper
    typedef enum { STEP_DIR_CCW    = 0x0a,
                   STEP_DIR_CW     = 0x05
    } STEP_DIRECTION_T;
    
    // legal directions for individual DC motors
    typedef enum { DIR_CCW    = 0x02,
                   DIR_CW     = 0x01
    } DC_DIRECTION_T;
    
    /**
     * grovemd constructor
     *
     * @param bus i2c bus to use
     * @param address i2c address to use
     */
    GroveMD(int bus=GROVEMD_I2C_BUS, 
            uint8_t address=GROVEMD_DEFAULT_I2C_ADDR);

    /**
     * GroveMD Destructor
     */
    ~GroveMD();

    /**
     * Compose and write a 3-byte packet to the controller
     *
     * @param reg register location
     * @param data1 first byte of data
     * @param data2 second byte of data
     * @return true if write successful
     */
    bool writePacket(REG_T reg, uint8_t data1, uint8_t data2);

    /**
     * For controlling DC motors, set the speeds of motors A & B.
     * Valid values are 0-255.
     *
     * @param speedA speed of motor A
     * @param speedB speed of motor B
     * @return true if command successful
     */
    bool setMotorSpeeds(uint8_t speedA, uint8_t speedB);

    /**
     * For controlling DC motors, set the PWM frequency prescale
     * factor.  Note this register is not ducumented other than to say
     * that the default value is 0x03.  Presumably this is the timer
     * pre-scale factor used on the ATMega MCU timer driving the PWM.
     *
     * @param freq PWM prescale frequency, default 0x03
     * @return true if command successful
     */
    bool setPWMFrequencyPrescale(uint8_t freq=0x03);

    /**
     * For controlling DC motors, set the directions of motors A & B
     *
     * @param dirA direction for motor A, DIR_CW or DIR_CCW
     * @param dirB direction for motor B, DIR_CW or DIR_CCW
     * @return true if command successful
     */
    bool setMotorDirections(DC_DIRECTION_T dirA, DC_DIRECTION_T dirB);

    /**
     * For controlling a stepper motor, set a direction, speed and
     * then enable.
     *
     * @param dir direction, STEP_DIR_CW or STEP_DIR_CCW
     * @param speed motor speed. Valid range is 1-255, higher is slower.
     * @return true if command successful
     */
    bool enableStepper(STEP_DIRECTION_T dir, uint8_t speed);

    /**
     * For controlling a stepper motor, stop the stepper motor.
     *
     * @return true if command successful
     */
    bool disableStepper();

    /**
     * For controlling a stepper motor, specify the number of steps to
     * execute.  Valid values are 1-255, 255 means to rotate continuously.
     *
     * @param steps number of steps to execute.  255 means rotate continously.
     * @return true if command successful
     */
    bool setStepperSteps(uint8_t steps);


  private:
    mraa_i2c_context m_i2c;
    uint8_t m_addr;
  };
}


