/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>

//! [Interesting]
#include "max31855.hpp"

int
main(int argc, char** argv)
{
    upm::MAX31855 temp(0, 8);

    std::cout << temp.getTemp() << std::endl;

    return 0;
}
//! [Interesting]
