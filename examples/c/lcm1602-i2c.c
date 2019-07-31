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
    lcm1602_context lcd = lcm1602_i2c_init(0, 0x20, true, 16, 2);

    if (!lcd)
    {
        printf("lcm1602_i2c_init() failed\n");
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
