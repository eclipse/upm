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

#include <iostream>
#include <unistd.h>

#include "es08a.hpp"

using namespace upm;

ES08A::ES08A (int pin) : Servo(pin) {
    m_name          = "ES08A";
    m_maxAngle      = 180.0;
    m_minPulseWidth = 600;
    m_maxPulseWidth = 2200;
}

ES08A::~ES08A() {

}
