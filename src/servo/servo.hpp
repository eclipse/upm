/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
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
#include <mraa/pwm.h>

namespace upm {

#define MIN_PULSE_WIDTH             600
#define MAX_PULSE_WIDTH             2500
#define PERIOD                      20000

#define HIGH                        1
#define LOW                         0

#define DEFAULT_WAIT_DISABLE_PWM    0

/**
 * @brief Servo library 
 *
 * The base Servo class provides routines for setting the angle of the shaft
 * as well as setting and getting the minimum and maximum pulse width and 
 * the maximum period.
 *
 * @defgroup servo libupm-servo
 * @ingroup seeed emax pwm servos gsk
 */
class Servo {
    public:
        /**
         * Instantiates a Servo object
         *
         * @param pin Servo pin number
         */
        Servo (int pin);

        /**
         * Instantiates a Servo object
         *
         * @param pin Servo pin number
         * @param minPulseWidth Minimum pulse width, in microseconds
         * @param maxPulseWidth Maximum pulse width, in microseconds
         */
        Servo (int pin, int minPulseWidth, int maxPulseWidth);

        /**
         * Instantiates a Servo object
         *
         * @param pin Servo pin number
         * @param minPulseWidth Minimum pulse width, in microseconds
         * @param maxPulseWidth Maximum pulse width, in microseconds
         * @param waitAndDisablePwm If 1, PWM is enabled only during the setAngle() execution
         * for a period of 1 second, and then turned back off. If 0, PWM remains on afterward.
         */
        Servo (int pin, int minPulseWidth, int maxPulseWidth, int waitAndDisablePwm);

        /**
         * Servo object destructor
         */
        ~Servo();

        /**
         * Sets the angle of the servo engine.
         *
         * @param angle Number between 0 and 180
         * @return 0 if successful, non-zero otherwise
         */
        mraa_result_t setAngle (int angle);

        /**
         * Halts PWM for this servo and allows it to move freely.
         */
        mraa_result_t haltPwm ();

        /**
         * Returns the name of the component
         *
         * @return Name of the component
         */
        std::string name()
        {
            return m_name;
        }

        /**
         * Sets the minimum pulse width
         *
         * @param width Minimum HIGH signal width
         */
        void setMinPulseWidth (int width);

        /**
         * Sets the maximum pulse width
         *
         * @param width Maximum HIGH signal width
         */
        void setMaxPulseWidth (int width);

        /**
         * Sets the maximum period width
         *
         * @param period PWM period width
         */
        void setPeriod (int period);

        /**
         * Returns the minimum pulse width
         *
         * @return Minimum pulse width
         */
        int getMinPulseWidth ();

        /**
         * Returns the maximum pulse width
         *
         * @return Maximum pulse width
         */
        int getMaxPulseWidth ();

        /**
         * Returns the maximum PWM period width
         *
         * @return Maximum PWM period width
         */
        int getPeriod ();

    protected:
        int calcPulseTraveling (int value);

        std::string         m_name;
        int                 m_servoPin;
        float               m_maxAngle;
        mraa_pwm_context    m_pwmServoContext;
        int                 m_currAngle;

        int                 m_minPulseWidth;
        int                 m_maxPulseWidth;
        int                 m_period;

        int                 m_waitAndDisablePwm;

    private:
        void init (int pin, int minPulseWidth, int maxPulseWidth, int waitAndDisablePwm);
};

}
