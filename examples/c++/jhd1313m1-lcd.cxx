/*
 * Author: Yevgeniy Kiveish <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <string>

#include "jhd1313m1.hpp"
#include "upm_utilities.h"

int shouldRun = true;

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
    // 0x62 RGB_ADDRESS, 0x3E LCD_ADDRESS
    upm::Jhd1313m1 lcd(0, 0x3E, 0x62);

    int ndx = 0;
    uint8_t rgb[7][3] = { { 0xd1, 0x00, 0x00 }, { 0xff, 0x66, 0x22 }, { 0xff, 0xda, 0x21 },
                          { 0x33, 0xdd, 0x00 }, { 0x11, 0x33, 0xcc }, { 0x22, 0x00, 0x66 },
                          { 0x33, 0x00, 0x44 } };
    while (shouldRun) {
        // Alternate rows on the LCD
        lcd.setCursor(ndx % 2, 0);
        // Change the color
        uint8_t r = rgb[ndx % 7][0];
        uint8_t g = rgb[ndx % 7][1];
        uint8_t b = rgb[ndx % 7][2];
        lcd.setColor(r, g, b);
        lcd.write("Hello World " + std::to_string(ndx));
        // Echo via printf
        printf("Hello World %d rgb: 0x%02x%02x%02x\n", ndx++, r, g, b);

        upm_delay(1);
    }

    //! [Interesting]
    return 0;
}
