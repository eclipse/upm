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

#include <unistd.h>
#include <signal.h>
#include <stdio.h>

#include "upm_utilities.h"
#include "lcdks.h"

bool shouldRun = true;

void sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}


int main(int argc, char **argv)
{
    signal(SIGINT, sig_handler);

//! [Interesting]

    // Instantiate a LCDKS (LCD Keypad Shield) using default pins

    // LCD connection:
    // LCD RS pin to digital pin 8
    // LCD Enable pin to digital pin 9
    // LCD D4 pin to digital pin 4
    // LCD D5 pin to digital pin 5
    // LCD D6 pin to digital pin 6
    // LCD D7 pin to digital pin 7
    // Keypad analog pin 0
    // Backlight GPIO -1 (disabled)

    // NOTE: The default pins do not include support for a gpio
    // controlled backlight.  If you need one, specify the correct pin
    // as the last argument instead of -1.
    lcdks_context lcd = lcdks_init(8, 9, 4, 5, 6, 7, 0, -1);

    if (!lcd)
    {
        printf("lcdks_init() failed.\n");
        return 1;
    }


    lcdks_set_cursor(lcd, 0, 0);
    lcdks_write(lcd, "LCDKS driver", 12);
    lcdks_set_cursor(lcd, 1, 2);
    lcdks_write(lcd, "Hello World", 11);

    // output current key value every second.
    while (shouldRun)
    {
        printf("Button value: %f\n", lcdks_get_key_value(lcd));
        upm_delay(1);
    }

    lcdks_close(lcd);

//! [Interesting]
    return 0;
}
