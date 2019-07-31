/*
 * The MIT License (MIT)
 *
 * Author: Your Full Name <your@email.address>
 * Copyright (c) <year> <copyright holder>
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */


#include "sensortemplate.hpp"

using namespace upm;

SensorTemplate::SensorTemplate(int i2c_bus): _i2c(i2c_bus) {}

std::string SensorTemplate::helloWorld() { return "Hello World";}
