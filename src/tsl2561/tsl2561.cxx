/*
 * Author: Nandkishor Sonar <Nandkishor.Sonar@intel.com>
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


#include <string>
#include <stdexcept>
#include <unistd.h>
#include "tsl2561.h"

using namespace upm;


TSL2561::TSL2561(int bus, uint8_t devAddr, uint8_t gain, uint8_t integrationTime)
                                                : m_i2ControlCtx(bus)
{
    m_controlAddr = devAddr;
    m_bus = bus;
    m_gain = gain ;
    m_integrationTime = integrationTime;

    m_name = "TSL2561- Digital Light Sensor";

    mraa::Result error = m_i2ControlCtx.address(m_controlAddr);
    if (error != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_address() failed");
        return;
    }

    // POWER UP.
    error = m_i2ControlCtx.writeReg(REGISTER_Control, CONTROL_POWERON);
    if (error != mraa::SUCCESS) {
        throw std::runtime_error(std::string(__FUNCTION__) +
                               ": Unable to power up TSL2561");
        return;
    }
    // Power on Settling time
    usleep(1000);

    // Gain & Integration time .
    error = m_i2ControlCtx.writeReg(REGISTER_Timing, m_gain | m_integrationTime);
    if (error != mraa::SUCCESS) {
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": Unable to set gain/time");
        return;
    }

    // Set interrupt threshold to default.
    error = m_i2ControlCtx.writeReg(REGISTER_Interrupt, 0x00);
    if (error != mraa::SUCCESS) {
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": Unable to set interrupt threshold");
        return;
    }
}

TSL2561::~TSL2561()
{
    // POWER DOWN
    m_i2ControlCtx.writeReg(REGISTER_Control, CONTROL_POWEROFF);
}

int
TSL2561::getLux()
{
    mraa::Result error = mraa::SUCCESS;
    int lux;
    uint16_t rawLuxCh0;
    uint16_t rawLuxCh1;
    uint8_t ch0_low, ch0_high, ch1_low, ch1_high;

    error = i2cReadReg(REGISTER_Channal0L, ch0_low);
    if (error != mraa::SUCCESS) {
        fprintf(stderr, "Error: Unable to read channel0L in getRawLux()\n");
        return error;
    }

    error = i2cReadReg(REGISTER_Channal0H, ch0_high);
    if (error != mraa::SUCCESS) {
        fprintf(stderr, "Error: Unable to read channel0H in getRawLux()\n");
        return error;
    }

    rawLuxCh0 = ch0_high*256+ch0_low;

    error= i2cReadReg(REGISTER_Channal1L, ch1_low);
    if (error != mraa::SUCCESS) {
        fprintf(stderr, "Error: Unable to read channel1L in getRawLux()\n");
        return error;
    }

    error = i2cReadReg(REGISTER_Channal1H, ch1_high);
    if (error != mraa::SUCCESS) {
        fprintf(stderr, "Error: Unable to read channel1H in getRawLux()\n");
        return error;
    }

    rawLuxCh1 = ch1_high*256+ch1_low;

    uint64_t scale = 0;

    switch (m_integrationTime)
    {
      case 0: // 13.7 msec
         scale = LUX_CHSCALE_TINT0;
      break;
      case 1: // 101 msec
         scale = LUX_CHSCALE_TINT1;
      break;
      default: // assume no scaling
         scale = (1 << LUX_CHSCALE);
      break;
    }

    // scale if gain is NOT 16X
    if (!m_gain) scale = scale << 4;

    uint64_t channel1 = 0;
    uint64_t channel0 = 0;
    // scale the channel values
    channel0 = (rawLuxCh0 * scale) >> LUX_CHSCALE;
    channel1 = (rawLuxCh1 * scale) >> LUX_CHSCALE;

    // find the ratio of the channel values (Channel1/Channel0)
    // protect against divide by zero
    unsigned long ratio1 = 0;
    if (channel0 != 0) ratio1 = (channel1 << (LUX_RATIOSCALE+1)) / channel0;

    // round the ratio value
    unsigned long ratio = (ratio1 + 1) >> 1;

    unsigned int b, m;

    // CS package
    // Check if ratio <= eachBreak ?
    if ((ratio >= 0) && (ratio <= LUX_K1C))
       {b=LUX_B1C; m=LUX_M1C;}
    else if (ratio <= LUX_K2C)
       {b=LUX_B2C; m=LUX_M2C;}
    else if (ratio <= LUX_K3C)
       {b=LUX_B3C; m=LUX_M3C;}
    else if (ratio <= LUX_K4C)
       {b=LUX_B4C; m=LUX_M4C;}
    else if (ratio <= LUX_K5C)
       {b=LUX_B5C; m=LUX_M5C;}
    else if (ratio <= LUX_K6C)
       {b=LUX_B6C; m=LUX_M6C;}
    else if (ratio <= LUX_K7C)
       {b=LUX_B7C; m=LUX_M7C;}
    else if (ratio > LUX_K8C)
       {b=LUX_B8C; m=LUX_M8C;}

    uint64_t tempLux = 0;
    tempLux = ((channel0 * b) - (channel1 * m));
    // do not allow negative lux value
    if (tempLux < 0) tempLux = 0;

    // round lsb (2^(LUX_SCALE-1))
    tempLux += (1 << (LUX_SCALE-1));

    // strip off fractional portion
    lux = tempLux >> LUX_SCALE;

    return lux;
}


mraa::Result
TSL2561::i2cWriteReg (uint8_t reg, uint8_t value)
{
    mraa::Result error = mraa::SUCCESS;

    // Start transmission to device
    error = m_i2ControlCtx.address (m_controlAddr);
    if (error != mraa::SUCCESS) {
        fprintf(stderr, "Error: on i2c bus address setup in i2cWriteReg()\n");
        return error;
    }
    // Write register to I2C
    error = m_i2ControlCtx.writeByte (reg);
    if (error != mraa::SUCCESS) {
        fprintf(stderr, "Error: on i2c bus write reg in i2cWriteReg()\n");
        return error;
    }

    // Write value to I2C
    error = m_i2ControlCtx.writeByte (value);
    if (error != mraa::SUCCESS) {
        fprintf(stderr, "Error: on i2c bus write value in i2cWriteReg()\n");
        return error;
    }

    usleep(100000);

    return error;
}

mraa::Result
TSL2561::i2cReadReg(uint8_t reg, uint8_t &data)
{
    mraa::Result error = mraa::SUCCESS;

    // Start transmission to device
    error = m_i2ControlCtx.address(m_controlAddr);
    if (error != mraa::SUCCESS) {
        fprintf(stderr, "Error: on i2c bus address setup in i2cReadReg()\n");
        return error;
    }

    // Send address of register to be read.
    error = m_i2ControlCtx.writeByte(reg);
    if (error != mraa::SUCCESS) {
        fprintf(stderr, "Error: on i2c bus write in i2cReadReg()\n");
        return error;
    }

    // Read byte.
    data = m_i2ControlCtx.readByte();

    usleep(10000);

    return error;
}
