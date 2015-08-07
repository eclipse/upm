/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
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
#include <string.h>
#include <algorithm>
#include <stdexcept>

#include "sm130.h"

using namespace upm;

SM130::SM130 (int bus, int devAddr, int rst, int dready) {
    mraa_result_t error = MRAA_SUCCESS;

    m_name = "SM130";

    m_i2cAddr = devAddr;
    m_bus = bus;

    if (!(m_i2Ctx = mraa_i2c_init(m_bus)))
      {
        throw std::invalid_argument(std::string(__FUNCTION__) + 
                                    ": mraa_i2c_init() failed");
      }

    mraa_result_t ret = mraa_i2c_address(m_i2Ctx, m_i2cAddr);
    if (ret != MRAA_SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) + 
                                    ": mraa_i2c_address() failed");
    }

    m_resetPinCtx = mraa_gpio_init (rst);
    if (m_resetPinCtx == NULL) {
        throw std::invalid_argument(std::string(__FUNCTION__) + 
                                    ": mraa_gpio_init(RESET) failed");
    }

    m_dataReadyPinCtx = mraa_gpio_init (dready);
    if (m_dataReadyPinCtx == NULL) {
        throw std::invalid_argument(std::string(__FUNCTION__) + 
                                    ": mraa_gpio_init(DATA READY) failed");
    }

    error = mraa_gpio_dir (m_resetPinCtx, MRAA_GPIO_OUT);
    if (error != MRAA_SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) + 
                                    ": mraa_gpio_dir(RESET) failed");
    }

    error = mraa_gpio_dir (m_dataReadyPinCtx, MRAA_GPIO_OUT);
    if (error != MRAA_SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) + 
                                    ": mraa_gpio_dir(DATA READY) failed");
    }
}

SM130::~SM130 () {
    mraa_result_t error = MRAA_SUCCESS;

    error = mraa_i2c_stop(m_i2Ctx);
    if (error != MRAA_SUCCESS) {
        mraa_result_print(error);
    }
    error = mraa_gpio_close (m_resetPinCtx);
    if (error != MRAA_SUCCESS) {
        mraa_result_print(error);
    }
    error = mraa_gpio_close (m_dataReadyPinCtx);
    if (error != MRAA_SUCCESS) {
        mraa_result_print(error);
    }
}

const char*
SM130::getFirmwareVersion () {
    // Send VERSION command and retry a few times if no response
    for (uint8_t n = 0; n < 10; n++) {
        sendCommand (CMD_VERSION);
        if (available() && getCommand() == CMD_VERSION)
        //  return versionString;
        usleep(100 * 1000);
    }

    return 0;
}

uint8_t
SM130::available () {
    // If in SEEK mode and using DREADY pin, check the status
    if (m_LastCMD == CMD_SEEK_TAG) {
        if (!mraa_gpio_read(m_dataReadyPinCtx)) {
            return false;
        }
    }

    // Set the maximum length of the expected response packet
    uint8_t len;
    switch(m_LastCMD) {
        case CMD_ANTENNA_POWER:
        case CMD_AUTHENTICATE:
        case CMD_DEC_VALUE:
        case CMD_INC_VALUE:
        case CMD_WRITE_KEY:
        case CMD_HALT_TAG:
        case CMD_SLEEP:
            len = 4;
            break;
        case CMD_WRITE4:
        case CMD_WRITE_VALUE:
        case CMD_READ_VALUE:
            len = 8;
        case CMD_SEEK_TAG:
        case CMD_SELECT_TAG:
            len = 11;
            break;
        default:
            len = SIZE_PACKET;
    }

    // If valid data received, process the response packet
    if (i2cRecievePacket(len) > 0) {
        // Init response variables
        m_TagType = m_TagLength = *m_TagString = 0;

        // If packet length is 2, the command failed. Set error code.
        errorCode = getPacketLength () < 3 ? m_Data[2] : 0;

        // Process command response
        switch (getCommand ()) {
            case CMD_RESET:
            case CMD_VERSION:
                // RESET and VERSION commands produce the firmware version
                len = std::min ((unsigned int) getPacketLength(), (unsigned int) sizeof(m_Version)) - 1;
                memcpy(m_Version, m_Data + 2, len);
                m_Version[len] = 0;
                break;

            case CMD_SEEK_TAG:
            case CMD_SELECT_TAG:
                // If no error, get tag number
                if(errorCode == 0 && getPacketLength () >= 6)
                {
                    m_TagLength = getPacketLength () - 2;
                    m_TagType = m_Data[2];
                    memcpy(m_TagNumber, m_Data + 3, m_TagLength);
                    arrayToHex (m_TagString, m_TagNumber, m_TagLength);
                }
                break;

            case CMD_AUTHENTICATE:
                break;

            case CMD_READ16:
                break;

            case CMD_WRITE16:
            case CMD_WRITE4:
                break;

            case CMD_ANTENNA_POWER:
                errorCode = 0;
                antennaPower = m_Data[2];
                break;

            case CMD_SLEEP:
                // If in SLEEP mode, no data is available
                return false;
        }

        // Data available
        return true;
    }
    // No data available
    return false;
}

uint16_t
SM130::i2cRecievePacket (uint32_t len) {
    int readByte = 0;

    mraa_i2c_address(m_i2Ctx, m_i2cAddr);
    readByte = mraa_i2c_read(m_i2Ctx, m_Data, len);

    if (readByte > 0) {
        // verify checksum if length > 0 and <= SIZE_PAYLOAD
        if (m_Data[0] > 0 && m_Data[0] <= SIZE_PAYLOAD)
        {
            uint8_t i, sum;
            for (i = 0, sum = 0; i <= m_Data[0]; i++) {
                sum += m_Data[i];
            }
            // return with length of response, or -1 if invalid checksum
            return sum == m_Data[i] ? m_Data[0] : -1;
        }
    }

    return readByte;
}

void
SM130::arrayToHex (char *s, uint8_t array[], uint8_t len) {
    for (uint8_t i = 0; i < len; i++) {
        *s++ = toHex(array[i] >> 4);
        *s++ = toHex(array[i]);
    }

    *s = 0;
}

char
SM130::toHex (uint8_t b) {
    b = b & 0x0f;

    return b < 10 ? b + '0' : b + 'A' - 10;
}

mraa_result_t
SM130::i2cTransmitPacket (uint32_t len) {
    mraa_result_t error = MRAA_SUCCESS;
    uint8_t sum = 0;

    // Save last command
    m_LastCMD = m_Data[0];

    // calculate the sum check
    for (int i = 0; i < len; i++) {
        sum += m_Data[i];
    }

    // placing the sum check to the last byte of the packet
    m_Data[len + 1] = sum;

    error = mraa_i2c_address (m_i2Ctx, m_i2cAddr);
    error = mraa_i2c_write (m_i2Ctx, m_Data, len + 1);

    return error;
}

mraa_result_t
SM130::sendCommand (uint8_t cmd) {
    m_Data[0] = 1;
    m_Data[1] = cmd;
    i2cTransmitPacket(2);
}

