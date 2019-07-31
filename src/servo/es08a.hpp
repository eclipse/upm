/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>
#include "servo.hpp"

namespace upm {

/**
 * @library servo
 * @sensor es08a
 * @comname High-sensitivity Mini Servo
 * @altname Grove Servo
 * @type servos
 * @man emax
 * @web https://www.seeedstudio.com/EMAX-9g-ES08A-High-Sensitive-Mini-Servo-p-760.html
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
 * @snippet servo-es08a.cxx Interesting
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
