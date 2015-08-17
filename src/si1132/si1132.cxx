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
#include <errno.h>

#include "si1132.h"


#define SI1132_ADDRESS                  0x60

/* REGISTER ADDRESSES */
#define SI1132_REG_PART_ID              0x00
#define SI1132_REG_INT_CFG              0x03
#define SI1132_REG_IRQ_ENABLE           0x04
#define SI1132_REG_HW_KEY               0x07
#define SI1132_REG_MEAS_RATE0           0x08
#define SI1132_REG_MEAS_RATE1           0x09
#define SI1132_REG_PARAM_WR             0x17
#define SI1132_REG_COMMAND              0x18
#define SI1132_REG_RESPONSE             0x20
#define SI1132_REG_ALS_VIS_DATA0        0x22
#define SI1132_REG_ALS_VIS_DATA1        0x23
#define SI1132_REG_PARAM_RD             0x2E
#define SI1132_REG_CHIP_STAT            0x30

/* REGISTER VALUES */
#define SI1132_PART_ID                  0x32
#define SI1132_HW_KEY_INIT              0x17

 /* COMMAND VALUES */
#define SI1132_COMMAND_RESET            0x01
#define SI1132_COMMAND_ALS_FORCE        0x06
#define SI1132_COMMAND_PARAM_QUERY      0x80
#define SI1132_COMMAND_PARAM_SET        0xA0
#define SI1132_COMMAND_ALS_AUTO         0x0E 

/* PARAMETER RAM ADDRESSES */
#define SI1132_PARAM_CHLIST             0x01
#define SI1132_PARAM_ALS_VIS_ADC_COUNT  0x10 
#define SI1132_PARAM_ALS_VIS_ADC_GAIN   0x11  
#define SI1132_PARAM_ALS_VIS_ADC_MISC   0x12   

/* PARAMETER RAM VALUES */
#define SI1132_PARAM_CHLIST_ENALSVIS    0x10
#define SI1132_PARAM_ALS_VIS_RANGE_STD  0x00
#define SI1132_PARAM_ALS_VIS_RANGE_HI   0x20



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
        fprintf(stderr, "SI1132: Read ID failed. Data = %02x\n", data);
        status = MRAA_ERROR_UNSPECIFIED;
        return status;
    } 

    // disable automatic updates
    uint16_t rate = 0;
    status = writeRegister16(SI1132_REG_MEAS_RATE0, rate);
    if (status != MRAA_SUCCESS)  {
        fprintf(stderr, "SI1132_REG_MEAS_RATE0 failed\n");
        return status;        
    }

    // reset device
    status = writeRegister8(SI1132_REG_COMMAND, SI1132_COMMAND_RESET);
    if (status != MRAA_SUCCESS) {
        fprintf(stderr, "SI1132: Reset failed.\n");
        status = MRAA_ERROR_UNSPECIFIED;
        return status;
    } 
    sleepMs(30);

    // start state machine
    status = writeRegister8(SI1132_REG_HW_KEY, SI1132_HW_KEY_INIT);
    if (status != MRAA_SUCCESS) {
        fprintf(stderr, "writeRegister8 failed\n");
        return status;        
    }
    // sleepMs(10);
    uint8_t regValue = 0;
    i2cReadRegister(SI1132_REG_HW_KEY, &regValue);
    if (regValue != SI1132_HW_KEY_INIT) {
        fprintf(stderr, "Si1132: Did not start\n");
        status = MRAA_ERROR_UNSPECIFIED;
        return status;        
    }

    /*
    uint8_t regValue;
    status = i2cReadRegister(SI1132_REG_CHIP_STAT, &regValue);
    if (status != MRAA_SUCCESS) {
        fprintf(stderr, "i2cReadRegister SI1132_REG_CHIP_STAT failed\n");
        return status;        
    }
    printf("CHIP_STAT = %d\n", regValue);
    */

    // enable visible light channel
    status = writeParam(SI1132_PARAM_CHLIST, SI1132_PARAM_CHLIST_ENALSVIS);

    // set visible light range for indoor lighting
    status = writeParam(SI1132_PARAM_ALS_VIS_ADC_MISC, SI1132_PARAM_ALS_VIS_RANGE_STD);    

    // set visible light gain to 8
    status = writeParam(SI1132_PARAM_ALS_VIS_ADC_GAIN, 3);    
    status = writeParam(SI1132_PARAM_ALS_VIS_ADC_COUNT, 3 << 4);    


    /*
    status = readParam(SI1132_PARAM_CHLIST, &regValue);    
    if (status != MRAA_SUCCESS) {
        fprintf(stderr, "i2cReadRegister SI1132_REG_PARAM_RD failed\n");
        return status;        
    }
    */
    // fprintf(stdout, "SI1132::reset completed ok\n");    
    return status;
}

mraa_result_t SI1132::getValue(float* value) {
    uint16_t rawValue;    
    status = runCommand(SI1132_COMMAND_ALS_FORCE);    
    if (status != MRAA_SUCCESS)    
        return status;
    status = i2cReadRegister(SI1132_REG_ALS_VIS_DATA0, &rawValue);
    if (rawValue < 256)
        rawValue = 0;
    else
        rawValue -= 256;
    *value = static_cast<float>(rawValue);
    return status;
}

bool SI1132::isConfigured() {
    return status == MRAA_SUCCESS;
}

mraa_result_t SI1132::writeRegister8(uint8_t reg, uint8_t value)
{
    return mraa_i2c_write_byte_data(i2c, value, reg);
}

mraa_result_t SI1132::writeRegister16(uint8_t reg, uint16_t value)
{
    return mraa_i2c_write_word_data(i2c, value, reg);
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
    status = writeRegister8(SI1132_REG_COMMAND, 0);
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

mraa_result_t SI1132::runCommand(uint8_t command)
{
    uint8_t response = 0;
    int sleepTimeMs = 5;
    int timeoutMs = 50;
    int waitTimeMs = 0;    
    status = clearResponseRegister();
    if (status != MRAA_SUCCESS)    
        return status;    
    status = writeRegister8(SI1132_REG_COMMAND, command);
    if (status != MRAA_SUCCESS)    
        return status;
     while (response == 0 && status == MRAA_SUCCESS && waitTimeMs < timeoutMs) {
        status = i2cReadRegister(SI1132_REG_RESPONSE, &response);
        // printf("."); fflush(stdout);
        sleepMs(sleepTimeMs);
        waitTimeMs += sleepTimeMs;
    }
    // printf("Reponse = %d\n", response); 
    if (response == 0) {
        status = MRAA_ERROR_UNSPECIFIED;
        fprintf(stderr, "Comand %d failed\n", command);
    }
    return status;
}

mraa_result_t SI1132::writeParam(uint8_t param, uint8_t value)
{
    status = writeRegister8(SI1132_REG_PARAM_WR, value);
    if (status != MRAA_SUCCESS)    
        return status;
    return runCommand(SI1132_COMMAND_PARAM_SET | param);
}

mraa_result_t SI1132::readParam(uint8_t param, uint8_t* value)
{
    status = runCommand(SI1132_COMMAND_PARAM_QUERY | param);
    if (status != MRAA_SUCCESS)    
        return status;
    return i2cReadRegister(SI1132_REG_PARAM_RD, value);
}


void SI1132::sleepMs(int mseconds)
{
    struct timespec sleepTime;

    sleepTime.tv_sec = mseconds / 1000; // Number of seconds
    sleepTime.tv_nsec = ( mseconds % 1000 ) * 1000000; // Convert fractional seconds to nanoseconds

    // Iterate nanosleep in a loop until the total sleep time is the original
    // value of the seconds parameter
    while ( ( nanosleep( &sleepTime, &sleepTime ) != 0 ) && ( errno == EINTR ) );
}
