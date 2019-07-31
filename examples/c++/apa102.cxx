/*
 * Author: Yannick Adam <yannick.adam@gmail.com>
 * Copyright (c) 2016 Yannick Adam
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include "apa102.hpp"

using namespace std;

int
main(int argc, char** argv)
{
    //! [Interesting]
    // Instantiate a strip of 30 LEDs on SPI bus 0
    upm::APA102 ledStrip(800, 0);

    // Set all LEDs to Red
    ledStrip.setAllLeds(31, 255, 0, 0);

    // Set a section (10 to 20) to blue
    ledStrip.setLeds(10, 20, 31, 0, 0, 255);

    // Set a single LED to green
    ledStrip.setLed(15, 31, 0, 255, 0);

    //! [Interesting]
    return 0;
}
