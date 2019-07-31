/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>

#include "speaker.hpp"

using namespace std;

int
main()
{
    //! [Interesting]
    // Instantiate a Speaker on digital pin D2
    upm::Speaker speaker(2);

    // Play all 7 of the lowest notes
    speaker.playAll();

    // Play a medium C-sharp
    speaker.playSound('c', true, "med");
    //! [Interesting]

    cout << "Exiting" << endl;

    return 0;
}
