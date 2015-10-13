/*
 * Author: Sergey Kiselev <sergey.kiselev@intel.com>
 * Author: Yevgeniy Kiveish <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 - 2015 Intel Corporation.
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

#include <lcm1602.h>

int
main(int argc, char **argv)
{
//! [Interesting]
    // LCD connection:
    // LCD RS pin to digital pin 8
    // LCD Enable pin to digital pin 13
    // LCD D4 pin to digital pin 2
    // LCD D5 pin to digital pin 3
    // LCD D6 pin to digital pin 4
    // LCD D7 pin to digital pin 5
    // LCD R/W pin to ground
    // 10K trimmer potentiometer:
    //   ends to +5V and ground
    //   wiper to LCD VO pin (pin 3)
    upm::Lcm1602 *lcd = new upm::Lcm1602(8, 13, 2, 3, 4, 5, 20, 2);
    lcd->setCursor(0,0);
    lcd->write("Hello World");
    lcd->setCursor(1,2);
    lcd->write("Hello World");

    printf("Sleeping for 5 seconds\n");
    sleep(5);
    delete lcd;
//! [Interesting]
    return 0;
}
