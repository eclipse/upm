/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <signal.h>
#include <stdio.h>

#include "lpd8806.hpp"
#include "upm_utilities.h"

void scanner(upm::LPD8806& sensor, uint8_t r, uint8_t g, uint8_t b, uint8_t wait);

int doWork = 0;

void
sig_handler(int signo)
{
    printf("got signal\n");
    if (signo == SIGINT) {
        printf("exiting application\n");
        doWork = 1;
    }
}

int
main(int argc, char** argv)
{
    //! [Interesting]
    upm::LPD8806 sensor(0, 7, 10);
    upm_delay_us(1000000);

    sensor.show();

    while (!doWork) {
        // Back-and-forth lights
        scanner(sensor, 127, 0, 0, 30); // red, slow
        scanner(sensor, 0, 0, 127, 15); // blue, fast
        upm_delay_us(1000000);
    }
    //! [Interesting]

    std::cout << "exiting application" << std::endl;

    return 0;
}

void
scanner(upm::LPD8806& sensor, uint8_t r, uint8_t g, uint8_t b, uint8_t wait)
{
    int i, j, pos, dir;

    pos = 0;
    dir = 1;

    for (i = 0; i < ((sensor.getStripLength() - 1) * 8); i++) {
        // Draw 5 pixels centered on pos.  setPixelColor() will clip
        // any pixels off the ends of the strip, no worries there.
        // we'll make the colors dimmer at the edges for a nice pulse
        // look
        sensor.setPixelColor(pos - 2, r / 4, g / 4, b / 4);
        sensor.setPixelColor(pos - 1, r / 2, g / 2, b / 2);
        sensor.setPixelColor(pos, r, g, b);
        sensor.setPixelColor(pos + 1, r / 2, g / 2, b / 2);
        sensor.setPixelColor(pos + 2, r / 4, g / 4, b / 4);

        sensor.show();
        upm_delay_us(wait * 1000);
        // If we wanted to be sneaky we could erase just the tail end
        // pixel, but it's much easier just to erase the whole thing
        // and draw a new one next time.
        for (j = -2; j <= 2; j++) {
            sensor.setPixelColor(pos + j, 0, 0, 0);
        }
        // Bounce off ends of strip
        pos += dir;
        if (pos < 0) {
            pos = 1;
            dir = -dir;
        } else if (pos >= sensor.getStripLength()) {
            pos = sensor.getStripLength() - 2;
            dir = -dir;
        }
    }
}
