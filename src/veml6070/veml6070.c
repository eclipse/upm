/*
 * Author: Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#include "veml6070.h"

veml6070_context veml6070_init(int bus) {
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
    // Setting up 2 contexts for the 2 addresses
    dev->address_seq1 = VEML6070_SEQ1_DATA_BUF_REG;
    dev->address_seq2 = VEML6070_SEQ2_DATA_BUF_REG;

    dev->i2c_seq1 = mraa_i2c_init(dev->i2c_bus_number);
    if (dev->i2c_seq1 == NULL){
        free(dev);
        return NULL;
    }

    dev->i2c_seq2 = mraa_i2c_init(dev->i2c_bus_number);
    if (dev->i2c_seq2 == NULL){
        free(dev);
        return NULL;
    }

    if (mraa_i2c_address(dev->i2c_seq1, dev->address_seq1) != MRAA_SUCCESS)
    {
        mraa_i2c_stop(dev->i2c_seq1);
        free(dev);
        return NULL;
    }

    if (mraa_i2c_address(dev->i2c_seq2, dev->address_seq2) != MRAA_SUCCESS)
    {
        mraa_i2c_stop(dev->i2c_seq2);
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
    seq_1 = mraa_i2c_read_byte(dev->i2c_seq1);
    if(seq_1 == -1)
        return -1;

    // reading seq2
    seq_2 = mraa_i2c_read_byte(dev->i2c_seq2);
    if (seq_2 == -1)
        return -1;

    intensity = (seq_1<<8)|seq_2;

    return intensity;
}

upm_result_t veml6070_set_integration_time(veml6070_context dev, veml6070_integration_time_t time) {
    uint8_t integrationTime = ((time<<2)|0x02);
    if(mraa_i2c_write(dev->i2c_seq2, &integrationTime, 1) != MRAA_SUCCESS) {
        return UPM_ERROR_UNSPECIFIED;
    }

    return UPM_SUCCESS;
}
