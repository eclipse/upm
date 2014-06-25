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
#include <mraa/pwm.h>
#include <mraa/aio.h>
#include <mraa/gpio.h>

#define MIN_PERIOD         500
#define MAX_PERIOD         1000
#define PULSEWIDTH         480

#define HIGH               1
#define LOW                0

namespace upm {

/**
 * @brief C++ API for StepMotor Drivers
 *
 * This file defines the stepmotor C++ interface for libstepmotor
 *
 * @snippet stepmotor.cxx Interesting
 */
class StepMotor {
    public:
        /**
         * Instanciates a StepMotor object
         *
         * @param dirPin direction pin
         * @param stePin steper pulse pin
         */
        StepMotor (int dirPin, int stePin);

        /**
         * StepMotor object destructor.
         */
        ~StepMotor ();

        /**
         * Set the speed of rotation
         *
         * @param speed rotation speed
         */
        void setSpeed (int speed);

        /**
         * Rotate motor forward
         *
         * @param ticks number of tickes the motor will move
         */
        mraa_result_t stepForward (int ticks);

        /**
         * Rotate motor backward
         *
         * @param ticks number of tickes the motor will move
         */
        mraa_result_t stepBackwards (int ticks);

    private:
        std::string         m_name;

        int                 m_dirPin;
        int                 m_stePin;
        int                 m_speed;

        mraa_gpio_context    m_dirPinCtx;
        mraa_pwm_context     m_pwmStepContext;

        mraa_result_t move (int ticks);
        mraa_result_t dirForward ();
        mraa_result_t dirBackwards ();
    };
}
