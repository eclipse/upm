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

#include "max44000.hpp"

int
main(int argc, char** argv)
{
    //! [Interesting]
    upm::MAX44000 sensor(0, ADDR);
    std::cout << "proximity value = " << sensor.getAmbient() << std::endl;
    //! [Interesting]

    std::cout << "exiting application" << std::endl;

    return 0;
}
