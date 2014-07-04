/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 * 
 * Based on SM130 library developed by Marc Boon <http://www.marcboon.com>
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
#pragma once

#include <string>
#include <mraa/aio.h>
#include <mraa/gpio.h>
#include <mraa/i2c.h>
#include <unistd.h>
#include <stdlib.h>

#define SIZE_PAYLOAD 18                 // maximum payload size of I2C packet
#define SIZE_PACKET (SIZE_PAYLOAD + 2)  // total I2C packet size, including length uint8_t and checksum

#define HIGH                  1
#define LOW                   0

namespace upm {

/**
 * @brief C++ API for SM130 RFID reader module
 *
 * This file defines the SM130 C++ interface for libsm130
 *
 */
class SM130 {
    
    uint8_t m_Data[SIZE_PACKET]; //!< packet data
	char    m_Version[8];      //!< version string
	uint8_t m_TagNumber[7];    //!< tag number as uint8_t array
	uint8_t m_TagLength;       //!< length of tag number in uint8_ts (4 or 7)
	char    m_TagString[15];   //!< tag number as hex string
	uint8_t m_TagType;         //!< type of tag
	char    errorCode;         //!< error code from some commands
	uint8_t antennaPower;      //!< antenna power level
	uint8_t m_LastCMD;         //!< last sent command
    
    public:
        static const uint8_t MIFARE_ULTRALIGHT = 1;
        static const uint8_t MIFARE_1K         = 2;
        static const uint8_t MIFARE_4K         = 3;

        static const uint8_t CMD_RESET         = 0x80;
        static const uint8_t CMD_VERSION       = 0x81;
        static const uint8_t CMD_SEEK_TAG      = 0x82;
        static const uint8_t CMD_SELECT_TAG    = 0x83;
        static const uint8_t CMD_AUTHENTICATE  = 0x85;
        static const uint8_t CMD_READ16        = 0x86;
        static const uint8_t CMD_READ_VALUE    = 0x87;
        static const uint8_t CMD_WRITE16       = 0x89;
        static const uint8_t CMD_WRITE_VALUE   = 0x8a;
        static const uint8_t CMD_WRITE4        = 0x8b;
        static const uint8_t CMD_WRITE_KEY     = 0x8c;
        static const uint8_t CMD_INC_VALUE     = 0x8d;
        static const uint8_t CMD_DEC_VALUE     = 0x8e;
        static const uint8_t CMD_ANTENNA_POWER = 0x90;
        static const uint8_t CMD_READ_PORT     = 0x91;
        static const uint8_t CMD_WRITE_PORT    = 0x92;
        static const uint8_t CMD_HALT_TAG      = 0x93;
        static const uint8_t CMD_SET_BAUD      = 0x94;
        static const uint8_t CMD_SLEEP         = 0x96;
    
         /**
         * Instanciates a SM130 object
         *
         * @param di data pin
         * @param dcki clock pin
         */
        SM130 (int bus, int devAddr, int rst, int dready);

        /**
         * SM130 object destructor
         */
        ~SM130 ();
        
        /**
         * Get the firmware version string.
         */
        const char* getFirmwareVersion ();
        
        /**
         * 	Checks for availability of a valid response packet.
         *
         *	This function should always be called and return true prior to using results
         *	of a command.
         *
         *	@returns	true if a valid response packet is available
         */
        uint8_t available ();
        
        /**
         * Returns the packet length, excluding checksum
         */
        uint8_t getPacketLength () { return this->m_Data[0]; };
        
        /**
         * Returns the last executed command
         */
        uint8_t getCommand () { return this->m_Data[1]; };

        /**
         * Return name of the component
         */
        std::string name()
        {
            return m_name;
        }
    private:
        std::string m_name;
        mraa_gpio_context m_resetPinCtx;
        mraa_gpio_context m_dataReadyPinCtx;
        
        int m_i2cAddr;
        int m_bus;
        mraa_i2c_context m_i2Ctx;
        
        void arrayToHex (char *s, uint8_t array[], uint8_t len);
        char toHex (uint8_t b);
        
        uint16_t i2cRecievePacket (uint32_t len);
        mraa_result_t i2cTransmitPacket (uint32_t len);
        mraa_result_t sendCommand (uint8_t cmd);
};

}
