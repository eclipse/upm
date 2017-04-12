/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "lcm1602.h"

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
