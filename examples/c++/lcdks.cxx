/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * The MIT License
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <signal.h>

#include "lcdks.hpp"
#include "upm_utilities.h"

using namespace std;

bool shouldRun = true;

void
sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}

int
main(int argc, char** argv)
{
    signal(SIGINT, sig_handler);

    //! [Interesting]

    // Instantiate a LCDKS (LCD Keypad Shield) using default pins

    // NOTE: The default pins do not include support for a gpio
    // controlled backlight.  If you need one, you will need to specify
    // all neccessary pins to the constructor.
    upm::LCDKS lcd;

    lcd.setCursor(0, 0);
    lcd.write("LCDKS driver");
    lcd.setCursor(1, 2);
    lcd.write("Hello World");

    // output current key value every second.
    while (shouldRun) {
        cout << "Button value: " << lcd.getKeyValue() << endl;
        upm_delay(1);
    }

    //! [Interesting]
    return 0;
}
