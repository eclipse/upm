/*
 * Author: Nandkishor Sonar <Nandkishor.Sonar@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * LIGHT-TO-DIGITAL CONVERTER [TAOS-TSL2561]
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
#include "tsl2561.h"

int main (int argc, char **argv)
{
	mraa_result_t error = MRAA_SUCCESS;
	upm::TSL2561 *sensor = NULL;
	int calculatedLux = 0;
	int loopCount = 100;

//! [Interesting]
	if (argc < 2) {
        printf("Provide loop count \n");
    } else {
        loopCount = atoi(argv[1]);
    }
    sensor = new upm::TSL2561();
    //sensor = new upm::TSL2561(0,  TSL2561_Address,  GAIN_16X,  INTEGRATION_TIME2_402MS);
    //sensor = new upm::TSL2561(0,  TSL2561_Address,  GAIN_16X,  INTEGRATION_TIME1_101MS);
    //sensor = new upm::TSL2561(0,  TSL2561_Address,  GAIN_16X,  INTEGRATION_TIME0_13MS);
    //sensor = new upm::TSL2561(0,  TSL2561_Address,  GAIN_0X,  INTEGRATION_TIME2_402MS);

    for(int i=0; i< loopCount; i++){
        error = sensor->getLux(calculatedLux);
        if (error != MRAA_SUCCESS) {
            fprintf(stderr, "Error: on i2c bus address setup in i2cReadReg()\n");
            return error;
        }

        fprintf(stdout, "Lux = %d\n", calculatedLux);
    }

//! [Interesting]

    delete(sensor);

    return (0);
}
