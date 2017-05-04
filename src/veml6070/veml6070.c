/*
 * Author: Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2017 Intel Corporation.
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
#include "veml6070.h"

veml6070_context veml6070_init(uint8_t bus, uint8_t address) {
    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        return NULL;
    }

    veml6070_context dev =
      (veml6070_context) malloc(sizeof(struct _veml6070_context));

    if (!dev)
    {
        return NULL;
    }

    dev->i2c_bus_number = bus;
    // setting up the control register address here
    dev->address = address;

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

    // reset the sensor here

    return dev;
}

void veml6070_close(veml6070_context dev) {
    free(dev);
}

int16_t veml6070_get_uv_intensity(veml6070_context dev) {
    int8_t seq_1, seq_2;
    int16_t intensity;

    // reading seq1
    dev->address = VEML6070_SEQ1_DATA_BUF_REG;

    if (mraa_i2c_address(dev->i2c, dev->address) != MRAA_SUCCESS)
    {
        printf("Unable to add seq 1 address\n");
        return -1;
    }

    seq_1 = mraa_i2c_read_byte(dev->i2c);
    if(seq_1 == -1)
        return -1;

    // reading seq2
    dev->address = VEML6070_SEQ2_DATA_BUF_REG;

    if (mraa_i2c_address(dev->i2c, dev->address) != MRAA_SUCCESS)
    {
        printf("Unable to add seq 2 address\n");
        return -1;
    }

    seq_2 = mraa_i2c_read_byte(dev->i2c);
    if (seq_2 == -1)
        return -1;

    intensity = (seq_1<<8)|seq_2;

    return intensity;
}

upm_result_t veml6070_set_integration_time(veml6070_context dev, veml6070_integration_time_t time) {
    // setting integration time address
    dev->address = VEML6070_CTRL_REG;

    if (mraa_i2c_address(dev->i2c, dev->address) != MRAA_SUCCESS)
    {
        return UPM_ERROR_UNSPECIFIED;
    }

    uint8_t integrationTime = ((time<<2)|0x02);
    if(mraa_i2c_write(dev->i2c, &integrationTime, 1) != MRAA_SUCCESS) {
        return UPM_ERROR_UNSPECIFIED;
    }

    return UPM_SUCCESS;
}