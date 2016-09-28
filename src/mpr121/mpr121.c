/*
 * Author: Jon Trulson <jtrulson@ics.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
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

#include "mpr121.h"

mpr121_context mpr121_init(int bus, uint8_t address) {
    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        return NULL;
    }

    mpr121_context dev = (mpr121_context)malloc(sizeof(struct _mpr121_context));

    if (!dev)
      return NULL;

    dev->bus = bus;
    dev->address = address;
    dev->i2c = mraa_i2c_init(dev->bus);

    if(dev->i2c == NULL){
        free(dev);
        return NULL;
    }

    if (mraa_i2c_address(dev->i2c, dev->address) != MRAA_SUCCESS){
        printf("error using i2c address: %x\n", dev->address);
        free(dev);
        return NULL;
    }

    dev->over_current_fault = false;
    return dev;
}

void mpr121_close(mpr121_context dev){
    mraa_i2c_stop(dev->i2c);
    free(dev);
}

upm_result_t mpr121_write_bytes(mpr121_context dev, uint8_t reg,
                                uint8_t *buffer, int len){
    if(!len || !buffer)
        return UPM_ERROR_INVALID_SIZE;

    // create a buffer 1 byte larger than the supplied buffer,
    // store the register in the first byte
    uint8_t buf2[len + 1];
    buf2[0] = reg;

    // copy in the buffer after the reg byte

    int i;
    for(i=1; i<(len + 1); i++)
        buf2[i] = buffer[i-1];

    if(mraa_i2c_address(dev->i2c, dev->address) != MRAA_SUCCESS){
        printf("write byte error address\n");
        return UPM_ERROR_OPERATION_FAILED;
    }

    if(mraa_i2c_write(dev->i2c, buf2, len+1) != MRAA_SUCCESS){
        printf("write byte error while writing\n");
        return UPM_ERROR_OPERATION_FAILED;
    }
    return UPM_SUCCESS;
}

upm_result_t mpr121_read_bytes(mpr121_context dev, uint8_t reg,
                               uint8_t *buffer, int len){
    if(!len || !buffer)
        return UPM_ERROR_INVALID_SIZE;

    // The usual m_i2c.read() does not work here, so we need to
    // read each byte individually.
    int i;
    for (i=0; i<len; i++)
        buffer[i] = mraa_i2c_read_byte_data(dev->i2c, reg+i);

    return UPM_SUCCESS;
}

upm_result_t mpr121_read_buttons(mpr121_context dev, uint32_t* states,
                                     int num){
    int len = 2;
    uint8_t buffer[2];

    // read in the 2 bytes at register 0x00-0x01, and setup the member
    // variables accordingly.
    mpr121_read_bytes(dev, 0x00, buffer, len);

    *states = (buffer[0] | ((buffer[1] & 0x1f) << 8));
    if (buffer[1] & 0x80)
        dev->over_current_fault = true;
    else
        dev->over_current_fault = false;

    return UPM_SUCCESS;
}

upm_result_t mpr121_config_an3944(mpr121_context dev){
    // Configure the mpr121 chip as recommended in the AN3944 MPR121
    // Quick Start Guide
    // First, turn off all electrodes by zeroing out the Electrode Configuration
    // register.
    // If this one fails, it's unlikely any of the others will succeed.
    uint8_t eleConf = 0x00;
    if (mpr121_write_bytes(dev, 0x5e, &eleConf, 1) != UPM_SUCCESS){
        printf("write to electrode configuration register failed\n");
        return UPM_ERROR_OPERATION_FAILED;
    }

    // Section A
    // Filtering when data is greater than baseline
    // regs 0x2b-0x2e

    uint8_t sectA[] = {0x01, 0x01, 0x00, 0x00};
    if (mpr121_write_bytes(dev, 0x2b, sectA, 4) != UPM_SUCCESS){
        printf("write to section a failed\n");
        return UPM_ERROR_OPERATION_FAILED;
    }

    // Section B
    // Filtering when data is less than baseline
    // regs 0x2f-0x32

    uint8_t sectB[] = {0x01, 0x01, 0xff, 0x02};
    if (mpr121_write_bytes(dev, 0x2f, sectB, 4) != UPM_SUCCESS){
        printf("write to section b failed\n");
        return UPM_ERROR_OPERATION_FAILED;
    }

    // Section C
    // Touch Threshold/Release registers, ELE0-ELE11
    // regs 0x41-0x58
    //                    __T_  __R_
    uint8_t sectC[] =  {0x0f, 0x0a,
        0x0f, 0x0a,
        0x0f, 0x0a,
        0x0f, 0x0a,
        0x0f, 0x0a,
        0x0f, 0x0a,
        0x0f, 0x0a,
        0x0f, 0x0a,
        0x0f, 0x0a,
        0x0f, 0x0a,
        0x0f, 0x0a,
        0x0f, 0x0a};

    if (mpr121_write_bytes(dev, 0x41, sectC, 24) != UPM_SUCCESS){
        printf("failed to configure touch threshold/release regs\n");
        return UPM_ERROR_OPERATION_FAILED;
    }

    // Section D
    // Filter configuration
    // reg 0x5d
    uint8_t filterConf = 0x04;
    if (mpr121_write_bytes(dev, 0x5d, &filterConf, 1) != UPM_SUCCESS){
        printf("unable to configure filters\n");
        return UPM_ERROR_OPERATION_FAILED;
    }

    // Section F
    // Autoconfiguration registers
    // regs 0x7b-0x7f
    uint8_t sectF0 = 0x0b;
    if (mpr121_write_bytes(dev, 0x7b, &sectF0, 1) != UPM_SUCCESS){
        printf("unable to configure auto config regs\n");
        return UPM_ERROR_OPERATION_FAILED;
    }

    uint8_t sectF1[] = {0x9c, 0x65, 0x8c};
    if (mpr121_write_bytes(dev, 0x7d, sectF1, 3) != UPM_SUCCESS){
        return UPM_ERROR_OPERATION_FAILED;
    }

    // Section E - this one must be set last, and switches to run mode
    // Enable all 12 electrodes, and set a pre-calibration to avoid
    // excessive calibration delay on startup.
    // reg 0x5e
    eleConf = 0x8c;
    if (mpr121_write_bytes(dev, 0x5e, &eleConf, 3) != UPM_SUCCESS){
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}
