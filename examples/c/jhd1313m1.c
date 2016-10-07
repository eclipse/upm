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

#include "jhd1313m1.h"
#include "upm_utilities.h"

int main(int argc, char **argv)
{
//! [Interesting]
    // initialize a JHD1313m1 on I2C bus 0, LCD address 0x3e, RGB
    // address 0x62
    jhd1313m1_context lcd = jhd1313m1_init(0, 0x3e, 0x62);

    if (!lcd)
    {
        printf("jhd1313m1_i2c_init() failed\n");
        return 1;
    }

    jhd1313m1_set_cursor(lcd, 0, 0);
    jhd1313m1_write(lcd, "Hello World 1", 13);

    upm_delay(3);

    // change background color to a dimmer pure green
    jhd1313m1_set_color(lcd, 0, 128, 0);

    jhd1313m1_set_cursor(lcd, 1, 0);
    jhd1313m1_write(lcd, "Hello World 2", 13);

    upm_delay(3);

    jhd1313m1_close(lcd);
//! [Interesting]

    return 0;
}
