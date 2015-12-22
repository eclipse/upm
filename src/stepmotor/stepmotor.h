/*
 * Authors: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 *          Mihai Tudor Panu <mihai.tudor.panu@intel.com>
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
#include <mraa/pwm.hpp>
#include <mraa/common.hpp>
#include <mraa/gpio.hpp>

#define OVERHEAD_US     6
#define MINPULSE_US     5

#define HIGH            1
#define LOW             0

namespace upm {
/**
 * @brief Stepper Motor library
 * @defgroup stepmotor libupm-stepmotor
 * @ingroup sparkfun generic gpio motor
 */
/**
 * @library stepmotor
 * @sensor stepmotor
 * @comname Stepper Motor
 * @altname EasyDriver Stepper Motor Driver
 * @type motor
 * @man sparkfun generic
 * @web http://www.schmalzhaus.com/EasyDriver/index.html
 * @con gpio
 *
 * @brief API for the Stepper Motor
 *
 * This module defines the Stepper Motor interface. It is compatible with
 * stepper motor drivers that use 2 pins to control the motor, like an Easy
 * Driver from Brian Schmalz or the STR driver series from Applied Motion. It
 * can also control an enable pin if one is available and connected.
 *
 * The implementation is synchronous and thus blocking while the stepper motor
 * is in motion. However it is possible to send the commands via threading and
 * the performance of the library will be very good given a low CPU load. On a
 * busy system though you will notice some jitter especially at higher speeds.
 * It is possible to reduce this effect to some extent by using smoothing
 * and/or microstepping on stepper drivers that support such features.
 *
 * @image html stepmotor.jpg
 * <br><em>EasyDriver Sensor image provided by SparkFun* under
 * <a href=https://creativecommons.org/licenses/by-nc-sa/3.0/>
 * CC BY-NC-SA-3.0</a>.</em>
 *
 * @snippet stepmotor.cxx Interesting
 */
class StepMotor {
    public:
        /**
         * Instantiates a StepMotor object.
         *
         * @param dirPin Direction GPIO pin
         * @param stePin Stepper pulse GPIO pin
         * @param steps Number of steps per revolution (Default 200)
         * @param enPin Enable pin if connected (Optional)
         */
        StepMotor (int dirPin, int stePin, int steps = 200, int enPin = -1);

        /**
         * StepMotor object destructor.
         */
         ~StepMotor ();

        /**
         * Can be used to enable/disable the stepper driver if an enable pin is
         * available and connected. Check your data sheet as some drivers might
         * have the enable logic inverted.
         *
         * @param flag true to enable or false to disable
         */
        void enable (bool flag);

        /**
         * Sets the rotation speed in rpm. Default 60 rpm.
         *
         * @param speed Rotation speed in rpm
         */
        void setSpeed (int speed);

        /**
         * Rotates the motor by the specified number of steps. Positive values
         * rotate clockwise and negative values rotate counter-clockwise.
         *
         * @param ticks Number of steps the motor moves
         */
        mraa::Result step (int ticks);

        /**
         * Rotates the motor forward (clockwise).
         *
         * @param ticks Number of steps the motor moves
         */
        mraa::Result stepForward (int ticks);

        /**
         * Rotates the motor backward (counter-clockwise).
         *
         * @param ticks Number of steps the motor moves
         */
        mraa::Result stepBackward (int ticks);

        /**
         * Sets the current position. Useful if the motor is not at 0 when the
         * driver is initialized.
         *
         * @param step Current position
         */
        void setPosition (int pos);

        /**
         * Gets the current position. This is cumulative and the result of all
         * the step commands sent to the motor.
         *
         * @return Stepper's position.
         */
        int getPosition ();

        /**
         * Gets the current step. This is relative to one revolution.
         *
         * @return Current step, ranges from 0 to number of steps per revolution.
         */
        int getStep ();

    private:
        std::string         m_name;

        mraa::Gpio          m_dirPinCtx;
        mraa::Gpio          m_stePinCtx;
        mraa::Gpio          *m_enPinCtx;

        int                 m_delay;
        int                 m_steps;
        int                 m_position;

        mraa::Result dirForward ();
        mraa::Result dirBackward ();
        void move ();
        void delayus (int us);
    };
}
