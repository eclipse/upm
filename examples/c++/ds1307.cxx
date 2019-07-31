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
#include <string>

#include "ds1307.hpp"

using namespace std;

void
printTime(upm::DS1307& rtc)
{
    cout << "The time is: " << rtc.month << "/" << rtc.dayOfMonth << "/" << rtc.year << " "
         << rtc.hours << ":" << rtc.minutes << ":" << rtc.seconds;

    if (rtc.amPmMode)
        cout << ((rtc.pm) ? " PM " : " AM ");

    cout << endl;

    cout << "Clock is in " << ((rtc.amPmMode) ? "AM/PM mode" : "24hr mode") << endl;
}

int
main(int argc, char** argv)
{
    //! [Interesting]
    // Instantiate a DS1037 on I2C bus 0
    upm::DS1307 rtc(0);

    // always do this first
    cout << "Loading the current time... " << endl;
    if (!rtc.loadTime()) {
        cerr << "rtc.loadTime() failed." << endl;
        return 0;
    }

    printTime(rtc);

    // set the year as an example
    cout << "setting the year to 50" << endl;
    rtc.year = 50;

    rtc.setTime();

    // reload the time and print it
    rtc.loadTime();
    printTime(rtc);

    //! [Interesting]

    return 0;
}
