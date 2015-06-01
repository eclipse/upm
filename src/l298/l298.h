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

#include <mraa/gpio.h>
#include <mraa/pwm.h>

// in milliseconds
#define L298_DEFAULT_PWM_PERIOD 4

namespace upm {
  /**
   * @brief UPM library for the L298 Dual H-Bridge Motor Driver
   * @defgroup l298 libupm-l298
   * @ingroup seeed sparkfun gpio pwm motor
   */

  /**
   * @library l298
   * @sensor l298
   * @comname L298 Dual H-Bridge Motor Driver
   * @type motor
   * @man seeed sparkfun
   * @con gpio pwm
   *
   * @brief API for the L298 Dual H-Bridge module
   *
   * It was developed using the RobotBase Dual H-Bridge module.
   *
   * This module can support 2 DC motors, or 1 2-phase stepper motor.
   * It requires 3 pins per DC motor (or h-bridge), or 4 pins for
   * the stepper (uses both h-bridges).  
   *
   * Example driving a DC motor
   * @snippet l298.cxx Interesting
   * Example driving a stepper motor
   * @snippet l298-stepper.cxx Interesting
   */
  class L298 {
  public:

    /**
     * Enum to specify the direction of a motor
     */
    typedef enum {
      DIR_NONE = 0x00,
      DIR_CW   = 0x01,
      DIR_CCW  = 0x02
    } L298_DIRECTION_T;

    /**
     * L298 constructor for DC motor(s) connected.  
     *
     * @param pwm digital pin to use for DC motor - must be PWM capable
     * @param dir1 digital pin to use for motor direction pin 1
     * @param dir2 digital pin to use for motor direction pin 2
     */
    L298(int pwm, int dir1, int dir2);

    /**
     * L298 constructor for 4-wire stepper motor
     *
     * @param stepsPerRev number of steps per full revolution
     * @param en enable pin
     * @param i1 digital pin to use for stepper input 1
     * @param i2 digital pin to use for stepper input 2
     * @param i3 digital pin to use for stepper input 3
     * @param i4 digital pin to use for stepper input 4
     */
    L298(int stepsPerRev, int en, int i1, int i2, int i3, int i4);

    /**
     * L298 Destructor
     */
    ~L298();

    /**
     * Return the number of milliseconds elapsed since initClock()
     * was last called.
     *
     * @return elapsed milliseconds
     */
    uint32_t getMillis();

    /**
     * Reset the Clock
     *
     */
    void initClock();

    /**
     * set the period in milliseconds
     *
     * @param ms period in milliseconds
     */
    void setPeriodMS(int ms);

    /**
     * enable PWM output for a motor
     *
     * @param enable enable PWM output if true, disable if false
     */
    void enable(bool enable);

    /**
     * set the speed of a DC or stepp motor.  For a DC motor, Values
     * can range from 0 (off) to 100 (full speed).  For a stepper
     * motor, specify the desired RPM.
     *
     * @param speed speed to set the motor to
     */
    void setSpeed(int speed);

    /**
     * set the direction of the motor, clockwise or counter clockwise
     *
     * @param dir direction to set the motor to
     */
    void setDirection(L298_DIRECTION_T dir);

    /**
     * step the stepper motor a specified number of steps
     *
     * @param steps number of steps to move the stepper motor
     */
    void stepperSteps(unsigned int steps);

  private:
    // DC motor mode enabled
    bool m_motor;
    // stepper mode enabled
    bool m_stepper;

    struct timeval m_startTime;

    // DC motor
    mraa_pwm_context m_pwm;
    mraa_gpio_context m_dir1;
    mraa_gpio_context m_dir2;

    // stepper (4-wire)
    mraa_gpio_context m_stepEnable;
    mraa_gpio_context m_stepI1;
    mraa_gpio_context m_stepI2;
    mraa_gpio_context m_stepI3;
    mraa_gpio_context m_stepI4;

    // steps per revolution
    int m_stepsPerRev;
    int m_currentStep;
    uint32_t m_stepDelay;

    /**
     * step the motor one tick
     *
     */
    void stepperStep();

    // step direction - 1 = forward, -1 = backward
    int m_stepDirection;
  };
}


