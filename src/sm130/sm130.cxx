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

#include "sm130.h"

using namespace upm;

struct SM130Exception : public std::exception {
    std::string message;
    SM130Exception (std::string msg) : message (msg) { }
    ~SM130Exception () throw () { }
    const char* what() const throw () { return message.c_str(); }
};

SM130::SM130 (int bus, int devAddr, int rst, int dready) {
    mraa_result_t error = MRAA_SUCCESS;

    this->m_name = "SM130";

    this->m_i2cAddr = devAddr;
    this->m_bus = bus;

    this->m_i2Ctx = mraa_i2c_init(this->m_bus);

    mraa_result_t ret = mraa_i2c_address(this->m_i2Ctx, this->m_i2cAddr);
    if (ret != MRAA_SUCCESS) {
        throw SM130Exception ("Couldn't initilize I2C.");
    }

    this->m_resetPinCtx = mraa_gpio_init (rst);
    if (m_resetPinCtx == NULL) {
        throw SM130Exception ("Couldn't initilize RESET pin.");
    }

    this->m_dataReadyPinCtx = mraa_gpio_init (dready);
    if (m_dataReadyPinCtx == NULL) {
        throw SM130Exception ("Couldn't initilize DATA READY pin.");
    }

    error = mraa_gpio_dir (this->m_resetPinCtx, MRAA_GPIO_OUT);
    if (error != MRAA_SUCCESS) {
        throw SM130Exception ("Couldn't set direction for RESET pin.");
    }

    error = mraa_gpio_dir (this->m_dataReadyPinCtx, MRAA_GPIO_OUT);
    if (error != MRAA_SUCCESS) {
        throw SM130Exception ("Couldn't set direction for DATA READY pin.");
    }
}

SM130::~SM130 () {
    mraa_result_t error = MRAA_SUCCESS;

    error = mraa_i2c_stop(this->m_i2Ctx);
    if (error != MRAA_SUCCESS) {
        mraa_result_print(error);
    }
    error = mraa_gpio_close (this->m_resetPinCtx);
    if (error != MRAA_SUCCESS) {
        mraa_result_print(error);
    }
    error = mraa_gpio_close (this->m_dataReadyPinCtx);
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
    if (this->m_LastCMD == CMD_SEEK_TAG) {
        if (!mraa_gpio_read(this->m_dataReadyPinCtx)) {
            return false;
        }
    }

    // Set the maximum length of the expected response packet
    uint8_t len;
    switch(this->m_LastCMD) {
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
    if (this->i2cRecievePacket(len) > 0) {
        // Init response variables
        this->m_TagType = this->m_TagLength = *this->m_TagString = 0;

        // If packet length is 2, the command failed. Set error code.
        errorCode = this->getPacketLength () < 3 ? this->m_Data[2] : 0;

        // Process command response
        switch (this->getCommand ()) {
            case CMD_RESET:
            case CMD_VERSION:
                // RESET and VERSION commands produce the firmware version
                len = std::min ((unsigned int) getPacketLength(), (unsigned int) sizeof(this->m_Version)) - 1;
                memcpy(this->m_Version, this->m_Data + 2, len);
                this->m_Version[len] = 0;
                break;

            case CMD_SEEK_TAG:
            case CMD_SELECT_TAG:
                // If no error, get tag number
                if(errorCode == 0 && this->getPacketLength () >= 6)
                {
                    this->m_TagLength = this->getPacketLength () - 2;
                    this->m_TagType = this->m_Data[2];
                    memcpy(this->m_TagNumber, this->m_Data + 3, this->m_TagLength);
                    this->arrayToHex (this->m_TagString, this->m_TagNumber, this->m_TagLength);
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
                antennaPower = this->m_Data[2];
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

    mraa_i2c_address(this->m_i2Ctx, this->m_i2cAddr);
    readByte = mraa_i2c_read(this->m_i2Ctx, this->m_Data, len);

    if (readByte > 0) {
        // verify checksum if length > 0 and <= SIZE_PAYLOAD
        if (this->m_Data[0] > 0 && this->m_Data[0] <= SIZE_PAYLOAD)
        {
            uint8_t i, sum;
            for (i = 0, sum = 0; i <= this->m_Data[0]; i++) {
                sum += this->m_Data[i];
            }
            // return with length of response, or -1 if invalid checksum
            return sum == this->m_Data[i] ? this->m_Data[0] : -1;
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
    this->m_LastCMD = this->m_Data[0];

    // calculate the sum check
    for (int i = 0; i < len; i++) {
        sum += this->m_Data[i];
    }

    // placing the sum check to the last byte of the packet
    this->m_Data[len + 1] = sum;

    error = mraa_i2c_address (this->m_i2Ctx, this->m_i2cAddr);
    error = mraa_i2c_write (this->m_i2Ctx, this->m_Data, len + 1);

    return error;
}

mraa_result_t
SM130::sendCommand (uint8_t cmd) {
    this->m_Data[0] = 1;
    this->m_Data[1] = cmd;
    this->i2cTransmitPacket(2);
}

