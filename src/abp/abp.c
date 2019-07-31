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
#include "abp.h"
 
abp_context abp_init(int bus, uint8_t dev_address){
    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        return NULL;
    }

    abp_context dev =
      (abp_context) malloc(sizeof(struct _abp_context));

    if (!dev)
    {
        return NULL;
    }

    // initialize the I2C bus
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

    // setting up defaults
    dev->abp_pressure_max = 5;
    dev->abp_pressure_min = 0;

    return dev;
}

void abp_close(abp_context dev){
    free(dev);
}

void abp_set_max_pressure(abp_context dev, int max) {
    dev->abp_pressure_max = max;
}

void abp_set_min_pressure(abp_context dev, int min) {
    dev->abp_pressure_min = min;
}

upm_result_t abp_update(abp_context dev) {
    int ret;

    // the first 4 bytes are of interest to us
    ret = mraa_i2c_read(dev->i2c, dev->readings, 4);
    if (ret == -1)
        return UPM_ERROR_OPERATION_FAILED;

    // the two MSBs are status bits
    //   S1    S0    Status
    //    0     0    Normal operation, valid data
    //    0     1    Device in command mode
    //    1     0    Stale Data
    //    1     1    Diagnostic condition
    uint8_t status = (dev->readings[0] >> 6) & 0x03;
    if(status == 1) {
        dev->status = COMMAND_MODE;
        return UPM_ERROR_UNSPECIFIED;
    } else if(status == 2) {
        dev->status = STALE_DATA;
        return UPM_ERROR_UNSPECIFIED;
    } else if(status == 3) {
        dev->status = DIAGNOSTIC;        
        return UPM_ERROR_UNSPECIFIED;
    } else
        dev->status = NORMAL_OPERATION;

    return UPM_SUCCESS;
}

float abp_get_pressure(abp_context dev) {
    float pressure_comp;

    if(dev->status != NORMAL_OPERATION) {
        printf("Error: Data might not be correct\n");
    }
    int output = (dev->readings[0]&ABP_PRESSURE_HIGH_BYTE_MASK)*256 + dev->readings[1];

    // Formula as per the data sheet
    // output(% of 2^14 counts) = (((output(90% counts) - output(10% counts))/Pmax - Pmin)*(Papplied - Pmin)) + output(10% count)
    // based on the formula we can calculate the pressure
    // Papplied = ((output(counts) - output(10% count))*(Pmax - Pmin))/(output(90% counts) - output(10% counts)) + Pmin
    pressure_comp = ((((float)output - OUTPUT_10_PERCENT_COUNT)*
                       (dev->abp_pressure_max - dev->abp_pressure_min))/
                       (OUTPUT_90_PERCENT_COUNT - OUTPUT_10_PERCENT_COUNT)) + 
                       dev->abp_pressure_min;

    return pressure_comp;
}

float abp_get_temperature(abp_context dev) {
    float temp_comp;

    if(dev->status != NORMAL_OPERATION) {
        printf("Error: Data might not be correct\n");
    }

    int output = (((dev->readings[2] << 8)|(dev->readings[3])) >> 
                        ABP_TEMPERATURE_LOW_BYTE_SHIFT)&ABP_TEMPERATURE_MASK;

    // Formula as per datasheet:
    // Temperature(C) = ((Output(decimal)/2047)*200) - 50
    temp_comp = (((float)output/2047)*200) - 50;

    return temp_comp;
}
