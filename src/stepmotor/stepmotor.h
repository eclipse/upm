/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Credits to Adafruit.
 * Based on Adafruit BMP085 library.
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
#include <math.h>
#include <mraa/pwm.hpp>
#include <mraa/aio.hpp>
#include <mraa/common.hpp>

#include <mraa/gpio.hpp>

#define MIN_PERIOD         500
#define MAX_PERIOD         1000
#define PULSEWIDTH         480

#define HIGH               1
#define LOW                0

namespace upm {
/**
 * @brief Stepper Motor library
 * @defgroup stepper libupm-stepper
 * @ingroup seeed sparkfun pwm gpio motor
 */
/**
 * @library stepmotor
 * @sensor stepmotor
 * @comname Stepper Motor
 * @altname EasyDriver Stepper Motor Driver
 * @type motor
 * @man seeed sparkfun
 * @web http://www.schmalzhaus.com/EasyDriver/index.html
 * @con pwm gpio
 *
 * @brief API for the Stepper Motor
 * 
 * This module defines the Stepper Motor interface. It is compatible with stepper
 * motor drivers that use 2 pins to control the motor, like an Easy Driver
 * from Brian Schmalz.
 *
 * @image html stepmotor.jpg
 * @snippet stepmotor.cxx Interesting
 */
class StepMotor {
    public:
        /**
         * Instantiates a StepMotor object
         *
         * @param dirPin Direction GPIO pin
         * @param stePin Stepper pulse PWM pin
         */
        StepMotor (int dirPin, int stePin);

        /**
         * StepMotor object destructor
         * no need for the destructor; all the connections will be
         * closed when m_dirPinCtx and m_pwmStepContext go out of
         * scope
         * ~StepMotor ();
         **/

        /**
         * Sets the rotation speed
         *
         * @param speed Rotation speed
         */
        void setSpeed (int speed);

        /**
         * Rotates the motor forward
         *
         * @param ticks Number of ticks the motor moves
         */
        mraa::Result stepForward (int ticks);

        /**
         * Rotates the motor backward
         *
         * @param ticks Number of ticks the motor moves
         */
        mraa::Result stepBackwards (int ticks);

    private:
        std::string         m_name;

        int                 m_dirPin;
        int                 m_stePin;
        int                 m_speed;

        mraa::Gpio    m_dirPinCtx;
        mraa::Pwm     m_pwmStepContext;

        mraa::Result move (int ticks);
        mraa::Result dirForward ();
        mraa::Result dirBackwards ();
    };
}
