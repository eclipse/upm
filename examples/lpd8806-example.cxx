/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
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

#include <unistd.h>
#include <iostream>
#include "lpd8806.h"
#include <signal.h>

void scanner(uint8_t r, uint8_t g, uint8_t b, uint8_t wait);

int doWork = 0;
upm::LPD8806 *sensor = NULL;

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
main(int argc, char **argv)
{
    //! [Interesting]
    sensor = new upm::LPD8806(10, 7);
    usleep (1000000);

    sensor->show ();

    while (!doWork) {
        // Back-and-forth lights
        scanner(127, 0, 0, 30);        // red, slow
        scanner(0, 0, 127, 15);        // blue, fast
        usleep (1000000);
    }
    //! [Interesting]

    std::cout << "exiting application" << std::endl;

    delete sensor;

    return 0;
}

void scanner(uint8_t r, uint8_t g, uint8_t b, uint8_t wait) {
    int i, j, pos, dir;

    pos = 0;
    dir = 1;

    for(i=0; i < ((sensor->getStripLength() - 1) * 8); i++) {
        // Draw 5 pixels centered on pos.  setPixelColor() will clip
        // any pixels off the ends of the strip, no worries there.
        // we'll make the colors dimmer at the edges for a nice pulse
        // look
        sensor->setPixelColor(pos - 2, r/4, g/4, b/4);
        sensor->setPixelColor(pos - 1, r/2, g/2, b/2);
        sensor->setPixelColor(pos, r, g, b);
        sensor->setPixelColor(pos + 1, r/2, g/2, b/2);
        sensor->setPixelColor(pos + 2, r/4, g/4, b/4);

        sensor->show();
        usleep (wait * 1000);
        // If we wanted to be sneaky we could erase just the tail end
        // pixel, but it's much easier just to erase the whole thing
        // and draw a new one next time.
        for(j=-2; j<= 2; j++) {
            sensor->setPixelColor(pos+j, 0,0,0);
        }
        // Bounce off ends of strip
        pos += dir;
        if(pos < 0) {
            pos = 1;
            dir = -dir;
        } else if (pos >= sensor->getStripLength()) {
            pos = sensor->getStripLength() - 2;
            dir = -dir;
        }
    }
}
