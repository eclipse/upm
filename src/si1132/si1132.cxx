/*
 * Author: Scott Ware <scott.r.ware@intel.com>
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

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#include "si1132.h"

using namespace upm;

SI1132::SI1132 (int bus) {

    i2c = mraa_i2c_init(bus);
    status = mraa_i2c_address(i2c, SI1132_ADDRESS);
    if (status != MRAA_SUCCESS) {
        fprintf(stderr, "SI1132: I2C bus failed to initialise.\n");
        return;
    }

    // Reset chip to defaults
    status = reset();
    if (status != MRAA_SUCCESS) {
        return;
    }

}

SI1132::~SI1132() {
    mraa_i2c_stop(i2c);
}

mraa_result_t SI1132::reset() {
    // Check version
    uint8_t data = mraa_i2c_read_byte_data(i2c, SI1132_REG_PART_ID);
    if (data != SI1132_PART_ID)  {
        fprintf(stderr, "SI1132: Reset failed. Data = %02x\n", data);
        status = MRAA_ERROR_UNSPECIFIED;
        return status;
    } 
    status = mraa_i2c_write_byte_data(i2c, SI1132_HW_KEY_INIT, SI1132_REG_HW_KEY);
    if (status != MRAA_SUCCESS)    
        return status;
    return mraa_i2c_write_word_data(i2c, 0, SI1132_REG_MEAS_RATE0);
    return status;
}

mraa_result_t SI1132::getValue(float* value) {
    uint16_t rawValue;    
    status = clearResponseRegister();
    if (status != MRAA_SUCCESS)    
        return status;
    status = i2cReadRegister(SI1132_REG_ALS_VIS_DATA0, &rawValue);
    *value = (float)rawValue;
    // status = MRAA_SUCCESS;
    return status;
}

bool SI1132::isConfigured() {
    return status == MRAA_SUCCESS;
}

mraa_result_t SI1132::i2cReadRegister(uint8_t reg, uint8_t* value)
{
    status = mraa_i2c_write_byte(i2c, reg);
    if (status != MRAA_SUCCESS)    
        return status;
    if (mraa_i2c_read(i2c, value, 1) != 1) {
        status = MRAA_ERROR_UNSPECIFIED;
        return status;        
    }
    status = MRAA_SUCCESS;
    return status;
}


mraa_result_t SI1132::i2cReadRegister(uint8_t reg, uint16_t* value)
{
    status = mraa_i2c_write_byte(i2c, reg);
    if (status != MRAA_SUCCESS)    
        return status;
    if (mraa_i2c_read(i2c, (uint8_t*)value, 2) != 2) {
        status = MRAA_ERROR_UNSPECIFIED;
        return status;        
    }
    status = MRAA_SUCCESS;
    return status;
}




mraa_result_t SI1132::clearResponseRegister()
{
    uint8_t regValue;
    status = mraa_i2c_write_byte_data(i2c, SI1132_REG_COMMAND, 0);
    if (status != MRAA_SUCCESS)    
        return status;
    status = i2cReadRegister(SI1132_REG_RESPONSE, &regValue) ;
    if (status != MRAA_SUCCESS)    
        return status;
    if (regValue != 0) {
        status = MRAA_ERROR_UNSPECIFIED;
        return status;        
    }
    status = MRAA_SUCCESS;
    return status;
}
