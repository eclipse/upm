/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include "lcm1602.h"
#include "upm_utilities.h"

int main(int argc, char **argv)
{
//! [Interesting]
    // LCD connection:
    // LCD RS pin to digital pin 8
    // LCD Enable pin to digital pin 13
    // LCD D4 pin to digital pin 2
    // LCD D5 pin to digital pin 3
    // LCD D6 pin to digital pin 4
    // LCD D7 pin to digital pin 5
    // (LCD R/W pin to ground - write only)
    // columns 16, rows 2
    lcm1602_context lcd = lcm1602_gpio_init(8, 13, 2, 3, 4, 5, 16, 2);

    if (!lcd)
    {
        printf("lcm1602_gpio_init() failed\n");
        return 1;
    }

    lcm1602_set_cursor(lcd, 0, 0);
    lcm1602_write(lcd, "Hello World 1", 13);

    upm_delay(3);

    lcm1602_set_cursor(lcd, 1, 0);
    lcm1602_write(lcd, "Hello World 2", 13);

    upm_delay(3);

    lcm1602_close(lcd);
//! [Interesting]

    return 0;
}
