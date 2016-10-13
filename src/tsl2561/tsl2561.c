/*
 * Author: Nandkishor Sonar <Nandkishor.Sonar@intel.com>,
 *         Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * LIGHT-TO-DIGITAL CONVERTER [TAOS-TSL2561]
 *   Inspiration and lux calculation formulas from data sheet
 *   URL: http://www.adafruit.com/datasheets/TSL2561.pdf
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

#include "tsl2561.h"

// forward declaration
upm_result_t tsl2561_compute_lux(const tsl2561_context dev, int *int_data);

tsl2561_context tsl2561_init(int bus, uint8_t dev_address, uint8_t gain,
                             uint8_t integration_time){
    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        return NULL;
    }

    tsl2561_context dev =
      (tsl2561_context)malloc(sizeof(struct _tsl2561_context));

    if (!dev)
        return NULL;

    dev->bus = bus;
    dev->address = dev_address;
    dev->gain = gain;
    dev->integration_time = integration_time;

    dev->i2c = mraa_i2c_init(dev->bus);
    if(dev->i2c == NULL){
        free(dev);
        return NULL;
    }

    if (mraa_i2c_address(dev->i2c, dev->address) != MRAA_SUCCESS)
        {
            mraa_i2c_stop(dev->i2c);
            free(dev);
            return NULL;
        }

    // POWER UP.
    if(mraa_i2c_write_byte_data(dev->i2c, CONTROL_POWERON, REGISTER_Control) != MRAA_SUCCESS){
        mraa_i2c_stop(dev->i2c);
        free(dev);
        return NULL;
    }

    // Power on Settling time
    upm_delay_us(1000);

    // Gain & Integration time.
    if(mraa_i2c_write_byte_data(dev->i2c, (dev->gain | dev->integration_time), REGISTER_Timing) != MRAA_SUCCESS){
        mraa_i2c_stop(dev->i2c);
        free(dev);
        return NULL;
    }

    // Set interrupt threshold to default.
    if(mraa_i2c_write_byte_data(dev->i2c, 0x00, REGISTER_Interrupt) != MRAA_SUCCESS){
        mraa_i2c_stop(dev->i2c);
        free(dev);
        return NULL;
    }

    return dev;
}

void tsl2561_close(tsl2561_context dev){
    if (mraa_i2c_write_byte_data(dev->i2c, CONTROL_POWEROFF,
                                 REGISTER_Control) != MRAA_SUCCESS){
        printf("Unable turn off device\n");
    }

    mraa_i2c_stop(dev->i2c);
    free(dev);
}

upm_result_t tsl2561_get_lux(const tsl2561_context dev, float* lux){
    int lux_val=0;

    tsl2561_compute_lux(dev, &lux_val);

    *lux = (float) lux_val;
    return UPM_SUCCESS;
}

upm_result_t tsl2561_i2c_write_reg(tsl2561_context dev, uint8_t reg,
                                   uint8_t value){
    // Start transmission to device
    if(mraa_i2c_address(dev->i2c, dev->address) != MRAA_SUCCESS){
        return UPM_ERROR_OPERATION_FAILED;
    }

    // Write register to I2C
    if(mraa_i2c_write_byte(dev->i2c, reg) != MRAA_SUCCESS){
        return UPM_ERROR_OPERATION_FAILED;
    }

    // Write value to I2C
    if(mraa_i2c_write_byte(dev->i2c, value) != MRAA_SUCCESS){
        return UPM_ERROR_OPERATION_FAILED;
    }

    upm_delay_ms(100);
    return UPM_SUCCESS;
}

upm_result_t tsl2561_i2c_read_reg(tsl2561_context dev, uint8_t reg,
                                  uint8_t* data){
   // Start transmission to dev
    if(mraa_i2c_address(dev->i2c, dev->address) != MRAA_SUCCESS){
        return UPM_ERROR_OPERATION_FAILED;
    }

    // Send address of register to be read.
    if(mraa_i2c_write_byte(dev->i2c, reg) != MRAA_SUCCESS){
        return UPM_ERROR_OPERATION_FAILED;
    }

    // Read byte.
    *data = mraa_i2c_read_byte(dev->i2c);

    //upm_delay(1);
    return UPM_SUCCESS;
}

upm_result_t tsl2561_compute_lux(const tsl2561_context dev, int *int_data) {
    int lux;
    uint16_t raw_lux_ch_0;
    uint16_t raw_lux_ch_1;
    uint8_t ch0_low, ch0_high, ch1_low, ch1_high;

    if (tsl2561_i2c_read_reg(dev, REGISTER_Channal0L, &ch0_low) != UPM_SUCCESS){
        return UPM_ERROR_OPERATION_FAILED;
    }

    if(tsl2561_i2c_read_reg(dev, REGISTER_Channal0H, &ch0_high) != UPM_SUCCESS){
        return UPM_ERROR_OPERATION_FAILED;
    }
    raw_lux_ch_0 = ch0_high*256 + ch0_low;

    if(tsl2561_i2c_read_reg(dev, REGISTER_Channal1L, &ch1_low) != UPM_SUCCESS){
        return UPM_ERROR_OPERATION_FAILED;
    }

    if(tsl2561_i2c_read_reg(dev, REGISTER_Channal1H, &ch1_high) != UPM_SUCCESS){
        return UPM_ERROR_OPERATION_FAILED;
    }
    raw_lux_ch_1 = ch1_high*256 + ch1_low;

    uint64_t scale = 0;

    switch(dev->integration_time){
        case 0: // 13.7 msec
            scale = LUX_CHSCALE_TINT0;
            break;
        case 1: // 101 msec
            scale = LUX_CHSCALE_TINT1;
            break;
        case 2: // assume no scaling
            scale = (1 << LUX_CHSCALE);
            break;
    }

    // scale if gain is NOT 16X
    if(!dev->gain)
        scale = scale << 4;

    uint64_t channel1 = 0;
    uint64_t channel0 = 0;

    // scale the channel values
    channel0 = (raw_lux_ch_0 * scale) >> LUX_CHSCALE;
    channel1 = (raw_lux_ch_1 * scale) >> LUX_CHSCALE;

    // find the ratio of the channel values (Channel1/Channel0)
    // protect against divide by zero
    uint64_t ratio_1 = 0;
    if (channel0 != 0)
        ratio_1 = (channel1 << (LUX_RATIOSCALE+1)) / channel0;

    // round the ratio value
    int64_t ratio = (ratio_1 + 1) >> 1;
    unsigned int b, m;

    // CS package
    // Check if ratio <= eachBreak ?
    if ((ratio >= 0) && (ratio <= LUX_K1C)){
        b=LUX_B1C; m=LUX_M1C;
    }
    else if (ratio <= LUX_K2C){
        b=LUX_B2C; m=LUX_M2C;
    }
    else if (ratio <= LUX_K3C){
        b=LUX_B3C; m=LUX_M3C;
    }
    else if (ratio <= LUX_K4C){
        b=LUX_B4C; m=LUX_M4C;
    }
    else if (ratio <= LUX_K5C){
        b=LUX_B5C; m=LUX_M5C;
    }
    else if (ratio <= LUX_K6C){
        b=LUX_B6C; m=LUX_M6C;
    }
    else if (ratio <= LUX_K7C){
        b=LUX_B7C; m=LUX_M7C;
    }
    else if (ratio > LUX_K8C){
        b=LUX_B8C; m=LUX_M8C;
    }
    int64_t temp_lux = 0;
    temp_lux = ((channel0 * b) - (channel1 * m));
    // do not allow negative lux value
    if (temp_lux < 0) temp_lux = 0;

    // round lsb (2^(LUX_SCALE-1))
    temp_lux += (1 << (LUX_SCALE-1));

    // strip off fractional portion
    lux = temp_lux >> LUX_SCALE;

    *int_data = lux;

    return UPM_SUCCESS;
}
