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

#include <stdint.h>
#include <sys/time.h>

#include <string>
#include <mraa/i2c.h>
#include <mraa/gpio.h>

#include "pca9685.h"

#define ADAFRUITMS1438_I2C_BUS 0
#define ADAFRUITMS1438_DEFAULT_I2C_ADDR 0x60

namespace upm {
  
  /**
   * @brief Adafruit Motor Shield 1438 library
   * @defgroup adafruitms1438 libupm-adafruitms1438
   * @ingroup adafruit i2c motor
   */

  /**
   * @library adafruitms1438
   * @sensor adafruitms1438
   * @comname Adafruit Motor Shield
   * @type motor
   * @man adafruit
   * @web http://www.adafruit.com/products/1438
   * @con i2c
   *
   * @brief API for the AdafruitMS1438 Motor Shield
   *
   * This class implements support for the stepper and DC motors that
   * can be connected to this Motor Shield.
   * Note: the two servo connections are not controlled by the PCA9685
   * controller (or this class). They are connected directly to digital
   * PWM pins 9 and 10 on the Arduino* breakout board.
   *
   * @image html adafruitms1438.jpg
   * An example using a DC motor connected to M3
   * @snippet adafruitms1438.cxx Interesting
   * An example using a stepper motor connected to M1 & M2
   * @snippet adafruitms1438-stepper.cxx Interesting
   */
  class AdafruitMS1438 {
  public:

    /**
     * Enum to specify the direction of a motor
     */
    typedef enum {
      DIR_NONE = 0x00,
      DIR_CW   = 0x01,
      DIR_CCW  = 0x02
    } DIRECTION_T;

    /**
     * Enum to specify a DC motor
     */
    typedef enum {
      MOTOR_M1 = 0,
      MOTOR_M2 = 1,
      MOTOR_M3 = 2,
      MOTOR_M4 = 3
    } DCMOTORS_T;

    /**
     * Enum to specify a stepper motor
     */
    typedef enum {
      STEPMOTOR_M12 = 0,
      STEPMOTOR_M34 = 1
    } STEPMOTORS_T;

    /**
     * AdafruitMS1438 constructor
     *
     * @param bus I2C bus to use
     * @param address Address for this sensor
     */
    AdafruitMS1438(int bus, uint8_t address = ADAFRUITMS1438_DEFAULT_I2C_ADDR);

    /**
     * AdafruitMS1438 destructor
     */
    ~AdafruitMS1438();

    /**
     * Returns the number of milliseconds elapsed since initClock(...)
     * was last called.
     *
     * @return Elapsed milliseconds
     */
    uint32_t getMillis(STEPMOTORS_T motor);

    /**
     * Resets the clock
     *
     */
    void initClock(STEPMOTORS_T motor);

    /**
     * Sets the PWM period.  Note: this applies to all PWM channels.
     *
     * @param hz Sets the PWM period 
     */
    void setPWMPeriod(float hz);

    /**
     * Enables PWM output for a motor
     *
     * @param motor DC motor to enable
     */
    void enableMotor(DCMOTORS_T motor);

    /**
     * Disables PWM output for a motor
     *
     * @param motor DC motor to disable
     */
    void disableMotor(DCMOTORS_T motor);

    /**
     * Enables output for a stepper motor
     *
     * @param motor Stepper motor to enable
     */
    void enableStepper(STEPMOTORS_T motor);

    /**
     * Disable output for a stepper motor
     *
     * @param motor Stepper motor to disable
     */
    void disableStepper(STEPMOTORS_T motor);

    /**
     * set the speed of a DC motor.  Values can range from 0 (off) to
     * 100 (full speed).
     *
     * @param motor the DC motor to configure
     * @param speed speed to set the motor to
     */
    void setMotorSpeed(DCMOTORS_T motor, int speed);

    /**
     * Sets the speed of a stepper in revolutions per minute (RPM)
     *
     * @param motor DC motor to configure
     * @param speed Speed to set the motor to
     */
    void setStepperSpeed(STEPMOTORS_T motor, int speed);

    /**
     * Sets the direction of a DC motor, clockwise or counterclockwise
     *
     * @param motor DC motor to configure
     * @param dir Direction to set the motor in
     */
    void setMotorDirection(DCMOTORS_T motor, DIRECTION_T dir);
 
    /**
     * Sets the direction of a stepper motor, clockwise or counterclockwise
     *
     * @param motor Stepper motor to configure
     * @param dir Direction to set the motor in
     */
    void setStepperDirection(STEPMOTORS_T motor, DIRECTION_T dir);
 
    /**
     * Sets a stepper motor configuration
     *
     * @param motor Stepper motor to configure
     * @param stepsPerRev Number of steps to complete a full revolution
     */
    void stepConfig(STEPMOTORS_T motor, unsigned int stepsPerRev);

    /**
     * Steps a stepper motor a specified number of steps
     *
     * @param motor Stepper motor to step
     * @param steps Number of steps to move the stepper motor
     */
    void stepperSteps(STEPMOTORS_T motor, unsigned int steps);

  private:
    // SWIG will generate a warning for these 'nested structs'; however,
    // it can be ignored as these structs are never exposed.

    // struct to hold mappings of DC motors
    typedef struct {
      int pwm;
      int in1;
      int in2;
    } DC_PINMAP_T;

    // struct to hold mappings of stepper motors
    typedef struct {
      int pwmA;
      int in1A;
      int in2A;
      int pwmB;
      int in1B;
      int in2B;
    } STEPPER_PINMAP_T;

    // struct to hold information about each stepper
    typedef struct {
      int stepsPerRev;          // steps per revolution
      int currentStep;          // current step number
      uint32_t stepDelay;       // delay between steps
      int stepDirection;        // direction to step
      struct timeval startTime; // starting time
    } STEPPER_CONFIG_T;

    void setupPinMaps();
    void stepperStep(STEPMOTORS_T motor);

    DC_PINMAP_T m_dcMotors[4];
    STEPPER_PINMAP_T m_stepMotors[2];
    STEPPER_CONFIG_T m_stepConfig[2];

    PCA9685 *m_pca9685;
  };
}


