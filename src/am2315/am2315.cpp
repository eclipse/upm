/*
 * Author: William Penner <william.penner@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <iostream>
#include <string>
#include <stdexcept>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <time.h>

#include "am2315.h"

using namespace upm;

char g_name[] = AM2315_NAME;

AM2315::AM2315(int bus, int devAddr) {
    m_temperature = 0;
    m_humidity    = 0;
    m_last_time = 0;

    m_name = g_name;

    m_controlAddr = devAddr;
    m_bus = bus;

    m_base_priority = sched_getscheduler(0);

    if ( !(m_i2ControlCtx = mraa_i2c_init(m_bus)) ) 
      {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_init() failed");
        return;
      }

    mraa_result_t ret = mraa_i2c_address(m_i2ControlCtx, m_controlAddr);
    if (ret != MRAA_SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_address() failed");
        return;
    }
    m_model = i2cReadReg_16(AM2315_MODEL);
    m_version = i2cReadReg_8(AM2315_VERSION);
    m_id = i2cReadReg_32(AM2315_ID);

    fprintf(stdout,"%s: Model: 0x%04x Version: 0x%02x ID: 0x%08x\n",
            m_name, m_model, m_version, m_id );
}

AM2315::~AM2315() {
    mraa_i2c_stop(m_i2ControlCtx);
}

void
AM2315::update_values(void)
{
    time_t ctime = time(NULL);
    if ((ctime - m_last_time) >= AM2315_SAMPLE) {
        uint32_t uival = i2cReadReg_32(AM2315_HUMIDITY);
        m_humidity = uival >> 16;
        m_temperature = uival & 0xffff;
        m_last_time = ctime;
    }
    else {
        // In case the time is changed - backwards
        if (ctime < m_last_time)
            m_last_time = ctime;
    }
}

float
AM2315::getTemperature(void)
{
    update_values();
    return (float)m_temperature / 10;
}

float
AM2315::getTemperatureF(void)
{
    return getTemperature() * 9 / 5 + 32;
}

float
AM2315::getHumidity(void)
{
    update_values();
    return (float)m_humidity / 10;
}

/*
 * Test function: when reading the AM2315 many times rapidly should
 * result in a temperature increase.  This test will verify that the
 * value is changing from read to read
 */

int
AM2315::testSensor(void)
{
    int i;
    int iError = 0;
    float fTemp, fHum;
    float fTempMax, fTempMin;
    float fHumMax, fHumMin;

    fprintf(stdout, "%s: Executing Sensor Test\n", m_name );

    fHum  = getHumidity();
    fTemp = getTemperature();
    fTempMax = fTempMin = fTemp;
    fHumMax  = fHumMin  = fHum;

    // Then sample the sensor a few times
    for (i=0; i < 10; i++) {
        fHum  = getHumidity();
        fTemp = getTemperature();
        if (fHum  < fHumMin)  fHumMin  = fHum;
        if (fHum  > fHumMax)  fHumMax  = fHum;
        if (fTemp < fTempMin) fTempMin = fTemp;
        if (fTemp > fTempMax) fTempMax = fTemp;
        usleep(50000);
    }

    // Now check the results
    if (fHumMin == fHumMax && fTempMin == fTempMax) {
        fprintf(stdout, "%s:  Humidity/Temp reading was unchanged - warning\n",
                m_name );
        iError++;
    }
    if (iError == 0) {
        fprintf(stdout, "%s:  Device appears functional\n", m_name );
    }

    fprintf(stdout, "%s: Test complete\n", m_name );

    return iError;
}

uint16_t
AM2315::crc16(uint8_t* ptr, uint8_t len)
{
    uint16_t crc = 0xffff;
    uint8_t i;

    while(len--) {
        crc ^= *ptr++;
        for (i=0; i < 8; i++) {
            if (crc & 0x01) {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else {
                crc >>= 1;
            }
        }
    }
    return crc;
}

/*
 * Functions to read and write data to the i2c device in the
 * special format used by the device.  This is using i2c to
 * interface to a controller that the AOSONG AM2315 uses to
 * perform the measurements and manage other registers.
 */
int
AM2315::i2cWriteReg(uint8_t reg, uint8_t* data, uint8_t ilen)
{
    uint8_t tdata[16] = { AM2315_WRITE, reg, ilen };
    mraa_result_t error;

    for (int i=0; i < ilen; i++) {
        tdata[i+3] = data[i];
    }
    uint16_t crc = crc16(tdata, ilen+3);
    // CRC is sent out backwards from other registers (low, high)
    tdata[ilen+3] = crc;
    tdata[ilen+4] = (crc >> 8);

    mraa_result_t ret = mraa_i2c_address(m_i2ControlCtx, m_controlAddr);
    int iLoops = 5;
    mraa_set_priority(HIGH_PRIORITY);
    do {
        error = mraa_i2c_write(m_i2ControlCtx, tdata, ilen+5);
        usleep(800);
    } while(error != MRAA_SUCCESS && --iLoops);
    mraa_set_priority(m_base_priority);

    if (error != MRAA_SUCCESS) {
        fprintf(stdout, "%s: Error, timeout writing sensor.\n", m_name);
        return -1;
    }
    crc = crc16(tdata,3);
    mraa_i2c_read(m_i2ControlCtx, tdata, 5);
    if ((tdata[0] != AM2315_WRITE) ||
        (tdata[1] != reg)          ||
        (tdata[2] != ilen)         ||
        (tdata[3] != (crc & 0xff)) ||
        (tdata[4] != (crc >> 8))) {
        fprintf(stdout, "%s: CRC error during write verification\n", m_name);
        return -1;
    }
    return 0;
}


// TODO: Need to patch up function to return only the data that
// is needed and not require the various functions that call this
// to send it enough buffer to cover the function

uint8_t
AM2315::i2cReadReg(int reg, uint8_t* data, int ilen)
{
    uint8_t tdata[16] = { AM2315_READ, reg, ilen };

    mraa_result_t ret = mraa_i2c_address(m_i2ControlCtx, m_controlAddr);
    int iLoops = 5;
    mraa_set_priority(HIGH_PRIORITY);
    do {
        ret = mraa_i2c_write(m_i2ControlCtx, tdata, 3);
        usleep(800);
    } while(ret != MRAA_SUCCESS && --iLoops);
    if (ret != MRAA_SUCCESS) {
        fprintf(stdout, "%s: Error, timeout reading sensor.\n", m_name);
        mraa_set_priority(m_base_priority);
        return -1;
    }
    usleep(5000);
    mraa_i2c_read(m_i2ControlCtx, tdata, ilen+4);
    mraa_set_priority(m_base_priority);

    uint16_t crc = crc16(tdata, ilen+2);
    if ((tdata[0] != AM2315_READ)  ||
        (tdata[1] != ilen)         ||
        (tdata[ilen+2] != (crc & 0xff)) ||
        (tdata[ilen+3] != (crc >> 8))) {
        fprintf(stdout, "%s: Read crc failed.\n", m_name);
    }
    for (int i=0; i < ilen; i++)
        data[i] = tdata[i+2];

    return 0;
}

/*
 * Functions to set up the reads and writes to simplify the process of
 * formatting data as needed by the microcontroller
 */

int
AM2315::i2cWriteReg_32(int reg, uint32_t ival) {
    uint8_t data[4];
    data[0] = ival >> 24;
    data[1] = ival >> 16;
    data[1] = ival >>  8;
    data[1] = ival & 0xff;
    return i2cWriteReg(reg, data, 4);
}

int
AM2315::i2cWriteReg_16(int reg, uint16_t ival) {
    uint8_t data[2];
    data[0] = ival & 0xff;
    data[1] = ival >> 8;
    return i2cWriteReg(reg, data, 2);
}

int
AM2315::i2cWriteReg_8(int reg, uint8_t ival) {
    uint8_t data[2];
    data[0] = ival & 0xff;
    data[1] = ival >> 8;
    return i2cWriteReg(reg, data, 2);
}

uint32_t
AM2315::i2cReadReg_32 (int reg) {
    uint8_t data[4];
    i2cReadReg(reg, data, 4);
    return ((((((uint32_t)data[0] << 8) | data[1]) << 8) |
            data[2]) << 8) | data[3];
}

uint16_t
AM2315::i2cReadReg_16 (int reg) {
    uint8_t data[2];
    i2cReadReg(reg, data, 2);
    return ((int16_t)data[0] << 8) | (uint16_t)data[1];
}

uint8_t
AM2315::i2cReadReg_8 (int reg) {
    uint8_t data[1];
    i2cReadReg(reg, data, 1);
    return data[0];
}

