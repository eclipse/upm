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
#include "servo.h"

namespace upm {

/**
 * @library servo
 * @sensor es08a
 * @comname ES08A Servo
 * @altname Grove Servo
 * @type servos
 * @man emax
 * @web http://www.seeedstudio.com/wiki/Grove_-_Servo
 * @con pwm
 * @kit gsk
 *
 * @brief API for the ES08A Servo
 * 
 * This module defines the ES08A interface for ES08A servos.
 * Like other servos, the ES08A servo has a shaft that can be controlled
 * by setting the desired angle. There are also routines for setting
 * and getting the minimum and maximum pulse width as well as the
 * maximum period.
 *
 * @image html es08a.jpg
 * @snippet es08a.cxx Interesting
 */
 class ES08A : public Servo {
    public:
        /**
         * Instantiates an ES08A object
         *
         * @param pin Servo pin number
         */
        ES08A (int pin);

        /**
         * ES08A object destructor
         */
        ~ES08A ();
};

}
