/*
 * Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Copyright (c) 2015 Intel Corporation.
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
 * @sensor es9257
 * @comname Micro Digital 3D Tail Servo
 * @altname Grove Servo
 * @type servos
 * @man emax
 * @web https://www.seeedstudio.com/EMAX-ES9257-2.5kg%26amp%3B-.05-sec-Micro-Digital-3D-Tail-Servo-p-762.html
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
