/*
 * Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
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
#include "servo.h"

namespace upm {

/**
 * @library servo
 * @sensor es9257
 * @comname ES9257 Servo
 * @altname Grove Servo
 * @type servos
 * @man emax
 * @web http://www.seeedstudio.com/wiki/Grove_-_Servo
 * @con pwm
 * @kit gsk
 *
 * @brief API for the ES9257 Servo
 * 
 * This module defines the ES9257 interface for ES9257 servos.
 * The ES9257 servo is a fast, heavy duty servo that is popular for moving the
 * control surfaces on RC models.
 * 
 * @image html es9257.jpg
 */
 class ES9257 : public Servo {
    public:
        /**
         * Instantiates an ES9257 object
         *
         * @param pin Servo pin number
         */
        ES9257 (int pin);

        /**
         * ES9257 object destructor
         */
        ~ES9257 ();
};

}
