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

#include <iostream>
#include <unistd.h>

#include "es9257.hpp"

using namespace upm;

ES9257::ES9257 (int pin) : Servo(pin) {
    m_name          = "ES9257";
    m_maxAngle      = 180.0;
    m_minPulseWidth = 475;
    m_maxPulseWidth = 2100;
}

ES9257::~ES9257() {

}
