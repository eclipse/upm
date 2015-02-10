/*
 * Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
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
#include "tp401.h"

using namespace std;

//! [Interesting]
// Give a qualitative meaning to the value from the sensor
std::string
airQuality(uint16_t value)
{
    if(value < 50) return "Fresh Air";
    if(value < 200) return "Normal Indoor Air";
    if(value < 400) return "Low Pollution";
    if(value < 600) return "High Pollution - Action Recommended";
    return "Very High Pollution - Take Action Immediately";
}

int main ()
{
    upm::TP401* airSensor = new upm::TP401(0); // Instantiate new grove air quality sensor on analog pin A0

    cout << airSensor->name() << endl;

    fprintf(stdout, "Heating sensor for 3 minutes...\n");
    // wait 3 minutes for sensor to warm up
    for(int i = 0; i < 3; i++) {
        if(i) {
            fprintf(stdout, "Please wait, %d minute(s) passed..\n", i);
        }
        sleep(60);
    }
    fprintf(stdout, "Sensor ready!\n");

    while(true) {
        uint16_t value = airSensor->getSample(); // Read raw value
        float ppm = airSensor->getPPM();    // Read CO ppm (can vary slightly from previous read)
        fprintf(stdout, "raw: %4d ppm: %5.2f   %s\n", value, ppm, airQuality(value).c_str());
        usleep(2500000);    // Sleep for 2.5s
    }

    delete airSensor;
    return 0;
}
//! [Interesting]
