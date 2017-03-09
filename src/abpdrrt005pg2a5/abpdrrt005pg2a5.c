/*
 * Author: Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2016 Intel Corporation.
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
#include "abpdrrt005pg2a5.h"
 
abpdrrt005pg2a5_context abpdrrt005pg2a5_init(uint8_t bus, uint8_t dev_address){
    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        return NULL;
    }

    abpdrrt005pg2a5_context dev =
      (abpdrrt005pg2a5_context) malloc(sizeof(struct _abpdrrt005pg2a5_context));

    if (!dev)
    {
        return NULL;
    }

    dev->i2c_bus_number = bus;
    dev->address = dev_address;

    dev->i2c = mraa_i2c_init(dev->i2c_bus_number);
    if (dev->i2c == NULL){
        free(dev);
        return NULL;
    }

    if (mraa_i2c_address(dev->i2c, dev->address) != MRAA_SUCCESS)
    {
        mraa_i2c_stop(dev->i2c);
        free(dev);
        return NULL;
    }

    return dev;
}

void abpdrrt005pg2a5_close(abpdrrt005pg2a5_context dev){
    free(dev);
}

upm_result_t abpdrrt005pg2a5_get_pressure_psi(abpdrrt005pg2a5_context dev, float* pressure_psi){
    uint8_t temp[2];
	int output;

    int16_t ret = mraa_i2c_read_word_data(dev->i2c, ABPDRRT005PG2A5_ADDRESS);
    if(ret == -1)
        return UPM_ERROR_OPERATION_FAILED;

    temp[0] = (uint8_t) ret;          // msb
	temp[1] = (uint8_t)(ret >> 8);               // lsb
	// Status bits should be 0 and 0, other values might mean a different mode or a wrong value
    uint8_t status = (temp[0] >> 6) & 0x03;
    if(status != 0)
        return UPM_ERROR_UNSPECIFIED;
    output = temp[0]*256 + temp[1];

    // Formula as per the data sheet
	// output(% of 2^14 counts) = (((output(90% counts) - output(10% counts))/Pmax - Pmin)*(Papplied - Pmin)) + output(10% count)
    // based on the formula we can calculate the pressure
    // Papplied = ((output(counts) - output(10% count))*(Pmax - Pmin))/(output(90% counts) - output(10% counts)) + Pmin
    *pressure_psi = ((((float)output - OUTPUT_10_PERCENT_COUNT)*(ABPDRRT005PG2A5_PRESSURE_MAX - ABPDRRT005PG2A5_PRESSURE_MIN))/(OUTPUT_90_PERCENT_COUNT - OUTPUT_10_PERCENT_COUNT)) + ABPDRRT005PG2A5_PRESSURE_MIN;

	return UPM_SUCCESS;
}

upm_result_t abpdrrt005pg2a5_get_pressure_pascal(abpdrrt005pg2a5_context dev, float* pressure_pascal){
    float int_pressure;

    if(abpdrrt005pg2a5_get_pressure_psi(dev, &int_pressure) != UPM_SUCCESS){
        return UPM_ERROR_OPERATION_FAILED;
    }

    // Conversion obtained from google
    *pressure_pascal = 6894.75729*int_pressure;

    return UPM_SUCCESS;
}