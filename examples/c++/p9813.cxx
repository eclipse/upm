/*
 * Author: Sergey Kiselev <sergey.kiselev@intel.com>
 * Copyright (c) 2017 Sergey Kiselev
 * Based on the p9813 driver writen by Yannick Adam <yannick.adam@gmail.com>
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include "p9813.hpp"

using namespace std;

int
main(int argc, char** argv)
{
    //! [Interesting]
    // Instantiate a chain of 2 LEDs using pin 2 for clock and pin 3 for data
    upm::P9813 ledChain(2, 2, 3);

    // Set all LEDs to Red
    ledChain.setAllLeds(255, 0, 0);

    // Set a single LED to green
    ledChain.setLed(1, 0, 255, 0);

    //! [Interesting]
    return 0;
}
