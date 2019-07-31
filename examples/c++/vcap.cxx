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

#include <iostream>
#include <string>

#include "vcap.hpp"

using namespace std;

int
main(int argc, char** argv)
{
    //! [Interesting]

    string defaultDev = "/dev/video0";

    // if an argument was specified, use it as the device instead
    if (argc > 1)
        defaultDev = string(argv[1]);

    cout << "Using device " << defaultDev << endl;
    cout << "Initializing..." << endl;

    // Instantiate an VCAP instance, using the specified video device
    upm::VCAP sensor(defaultDev);

    // enable some debug/verbose output
    sensor.setDebug(true);

    // This is just a hint.  The kernel can change this to a lower
    // resolution that the hardware supports.  Use getWidth() and
    // getHeight() methods to see what the kernel actually chose if you
    // care.
    sensor.setResolution(1920, 1080);

    // capture an image
    sensor.captureImage();

    // convert and save it as a jpeg
    sensor.saveImage("video-img1.jpg");

    cout << "Exiting..." << endl;

    //! [Interesting]

    return 0;
}
