/*
*
* Author: Rafael da Mata Neri <rafael.neri@gmail.com>
* Copyright (c) 2015 Intel Corporation.
*
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/
#include <iostream>

//! [Interesting]
#include "hx711.hpp"

int
main(int argc, char** argv)
{
    upm::HX711 scale(3, 2);

    // 2837: value obtained via calibration
    scale.setScale(2837);
    scale.tare();
    std::cout << scale.getUnits() << std::endl;

    return 0;
}
//! [Interesting]
