/*
 * Author: Henry Bruce <henry.bruce@intel.com>
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

#include <iostream>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "ms5611.hpp"


// Used to prevent silent failure in mraa_i2c_write()
// Value come from I2C_SMBUS_I2C_BLOCK_MAX in i2c driver
#define MAX_I2C_WRITE_SIZE 32

#define MS5611_CMD_ADC_READ           (0x00)
#define MS5611_CMD_RESET              (0x1E)
#define MS5611_CMD_CONV_D1            (0x40)
#define MS5611_CMD_CONV_D2            (0x50)
#define MS5611_CMD_READ_PROM          (0xA0)

#define MS5611_OSR_ULTRA_LOW_POWER    0x00
#define MS5611_OSR_LOW_POWER          0x02
#define MS5611_OSR_STANDARD           0x04
#define MS5611_OSR_HIGH_RES           0x06
#define MS5611_OSR_ULTRA_HIGH_RES     0x08

#define MS5611_PROM_SIZE              8


using namespace upm;


MS5611::MS5611(int i2cBus, int address)
{
    status = mraa::ERROR_INVALID_RESOURCE;
    i2c = new mraa::I2c(2);
    this->address = address;
    i2c->address(address);
    prom = new uint16_t[MS5611_PROM_SIZE];
    status = i2c->writeByte(MS5611_CMD_RESET);
    if (status != mraa::SUCCESS)
        UPM_THROW("Reset failed.");
    delayms(5);
    for (int i = 0; i < MS5611_PROM_SIZE; ++i) {
        uint8_t buf[2];
        int bytesRead = i2c->readBytesReg(MS5611_CMD_READ_PROM + 2*i, buf, 2);
        if (bytesRead != 2)
           UPM_THROW("PROM address failed.");
        prom[i] = buf[0] << 8;
        prom[i] |= buf[1];
        // printf("Read PROM entry %d = %04x\n", i, prom[i]);
    }

    // printf("CRC = %X\n", promCrc4());
    if (promCrc4() != (prom[7] & 0x000F))
        UPM_THROW("PROM checksum error.");
    setOverSampling(ULTRA_HIGH_RES);
}


MS5611::~MS5611()
{
    delete[] prom;
    delete i2c;
}


int MS5611::promCrc4() {
    unsigned int n_rem;
    unsigned int crc_read;
    unsigned char n_bit;
    n_rem = 0x00;
    crc_read = prom[7];
    prom[7]=(0xFF00 & (prom[7]));
    for (int cnt = 0; cnt < 16; cnt++) {
            if (cnt%2 == 1) {
                n_rem ^= (unsigned short) ((prom[cnt>>1]) & 0x00FF);
            } else {
                n_rem ^= (unsigned short) (prom[cnt>>1]>>8);
            }
            for (n_bit = 8; n_bit > 0; n_bit--) {
                if (n_rem & (0x8000)) {
                    n_rem = (n_rem << 1) ^ 0x3000;
                } else {
                    n_rem = (n_rem << 1);
                }
            }
        }
    n_rem= (0x000F & (n_rem >> 12));
    prom[7]=crc_read;
    return (n_rem ^ 0x0);
}


// Set oversampling value
void MS5611::setOverSampling(OsrMode osrMode)
{
    switch (osrMode)
    {
    case ULTRA_LOW_POWER:
        osr = MS5611_OSR_ULTRA_LOW_POWER;
        samplePeriod = 1;
        break;
    case LOW_POWER:
        osr = MS5611_OSR_LOW_POWER;
        samplePeriod = 2;
        break;
    case STANDARD:
        osr = MS5611_OSR_STANDARD;
        samplePeriod = 3;
        break;
    case HIGH_RES:
        osr = MS5611_OSR_HIGH_RES;
        samplePeriod = 5;
        break;
    case ULTRA_HIGH_RES:
        osr = MS5611_OSR_ULTRA_HIGH_RES;
        samplePeriod = 10;
        break;
     }
}


void MS5611::delayms(int millisecs)
{
   struct timespec sleepTime, rem;
   uint64_t nanosecs = 1000ULL * millisecs;
   sleepTime.tv_sec = millisecs / 1000;
   sleepTime.tv_nsec = (millisecs % 1000) * 1000000;
   while ((nanosleep( &sleepTime, &sleepTime ) != 0 ) && ( errno == EINTR ) );
}

uint32_t MS5611::readADC(int adcReg)
{
    uint32_t value;
    uint8_t buf[3];
    i2c->address(address);
    status = i2c->writeByte(adcReg + osr);
    if (status != mraa::SUCCESS)
       UPM_THROW("Convert D2 failed");
    delayms(100);
    int bytesRead = i2c->readBytesReg(MS5611_CMD_ADC_READ, buf, 3);
    if (bytesRead != 3)
       UPM_THROW("ADC read failed");
    // printf("%02X%02X%02X\n", buf[0], buf[1], buf[2]);
    value = ((uint32_t)buf[0] << 16) | ((uint32_t)buf[1] << 8) | buf[2];
    return value;
}

uint32_t MS5611::readRawTemperature()
{
    return readADC(MS5611_CMD_CONV_D2);
}

uint32_t MS5611::readRawPressure()
{
    return readADC(MS5611_CMD_CONV_D1);
}

int MS5611::getTemperatureCelsius()
{
    uint32_t D2 = readRawTemperature();
    int64_t dT = D2 - ((uint64_t)prom[5] << 8);
    int32_t temp = 2000 + ((int64_t)dT * (int64_t)prom[6]) / (int64_t)(1 << 23);
    return (temp + 50) / 100;
}


int MS5611::getPressurePa()
{
    int32_t rawTemp = readRawTemperature();
    int32_t rawPressure = readRawPressure();
    int64_t dT = rawTemp - ((uint64_t)prom[5] << 8);
    int64_t offset  = ((uint32_t)prom[2] << 16) + ((dT * (prom[4]) >> 7));     //was  OFF  = (C[2] << 17) + dT * C[4] / (1 << 6);
    int64_t scaler = ((uint32_t)prom[1] << 15) + ((dT * (prom[3]) >> 8));     //was  SENS = (C[1] << 16) + dT * C[3] / (1 << 7);
    int32_t temp = 2000 + (int64_t)dT * (int64_t)prom[6] / (int64_t)(1 << 23);

    if(temp < 2000) {
        float T1 = (temp - 2000) * (temp - 2000);
        int64_t offsetDelta  = (5 * T1) / 2;
        int64_t scalerDelta = (5 * T1) / 4;

        if(temp < -1500) {
            T1 = (temp + 1500) * (temp + 1500);
            offsetDelta  += 7 * T1;
            scalerDelta += 11 * T1 / 2;
        }
        offset -= offsetDelta;
        scaler -= scalerDelta;
    }
    int pressure = ((((int64_t)rawPressure * scaler ) >> 21) - offset) / (double) (1 << 15);
    return pressure;
}

