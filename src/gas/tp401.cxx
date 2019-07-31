/*
 * Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include "tp401.hpp"

using namespace upm;

TP401::TP401 (int gasPin) : Gas (gasPin) {
    m_name = "Grove Air Quality Sensor";
}

TP401::~TP401 () {
}

float
TP401::getPPM() {
    return 25.0 * (float)TP401::getSample() / 1023.0;
}
