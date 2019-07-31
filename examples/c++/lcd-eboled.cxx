/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>

#include "eboled.hpp"
#include "upm_utilities.h"

using namespace std;

int
main(int argc, char** argv)
{
    //! [Interesting]
    // Instantiate an Edison Block OLED using default values
    upm::EBOLED lcd;

    lcd.clear();
    lcd.setCursor(10, 15);
    lcd.write("Hello");
    lcd.setCursor(30, 15);
    lcd.write("World!");
    lcd.refresh();

    cout << "Sleeping for 5 seconds..." << endl;
    upm_delay(5);

    //! [Interesting]
    return 0;
}
