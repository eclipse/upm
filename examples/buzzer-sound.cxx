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
#include <signal.h>
#include "buzzer.h"


int
main(int argc, char **argv) {
    //! [Interesting]
    int chord[] = { DO, RE, MI, FA, SOL, LA, SI, DO, SI };

    // create Buzzer instance
    upm::Buzzer* sound = new upm::Buzzer(5);
    // print sensor name
    std::cout << sound->name() << std::endl;

    // play sound (DO, RE, MI, etc...), pausing for 0.1 seconds between notes
    for (int chord_ind = 0; chord_ind < 7; chord_ind++) {
        // play each note for one second
        std::cout << sound->playSound(chord[chord_ind], 1000000) << std::endl;
        usleep(100000);
    }
    //! [Interesting]

    std::cout << "exiting application" << std::endl;

    delete sound;

    return 0;
}
