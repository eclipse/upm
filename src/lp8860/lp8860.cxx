/*
 * Author: Henry Bruce <henry.bruce@intel.com>
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
#include <string.h>
#include "lp8860.hpp"
#include "mraa-utils.hpp"

// Used to prevent silent failure in mraa_i2c_write()
// Value come from I2C_SMBUS_I2C_BLOCK_MAX in i2c driver
#define MAX_I2C_WRITE_SIZE 32

#define LP8860_I2C_ADDR 0x2D

#define LP8860_DISP_CL1_BRT_MSB		0x00
#define LP8860_DISP_CL1_BRT_LSB		0x01
#define LP8860_DISP_CL1_CURR_MSB	0x02
#define LP8860_DISP_CL1_CURR_LSB	0x03
#define LP8860_CL2_BRT_MSB		0x04
#define LP8860_CL2_BRT_LSB		0x05
#define LP8860_CL2_CURRENT		0x06
#define LP8860_CL3_BRT_MSB		0x07
#define LP8860_CL3_BRT_LSB		0x08
#define LP8860_CL3_CURRENT		0x09
#define LP8860_CL4_BRT_MSB		0x0a
#define LP8860_CL4_BRT_LSB		0x0b
#define LP8860_CL4_CURRENT		0x0c
#define LP8860_CONFIG			0x0d
#define LP8860_STATUS			0x0e
#define LP8860_FAULT			0x0f
#define LP8860_LED_FAULT		0x10
#define LP8860_FAULT_CLEAR		0x11
#define LP8860_ID			0x12
#define LP8860_TEMP_MSB			0x13
#define LP8860_TEMP_LSB			0x14
#define LP8860_DISP_LED_CURR_MSB	0x15
#define LP8860_DISP_LED_CURR_LSB	0x16
#define LP8860_DISP_LED_PWM_MSB		0x17
#define LP8860_DISP_LED_PWM_LSB		0x18
#define LP8860_EEPROM_CNTRL		0x19
#define LP8860_EEPROM_UNLOCK		0x1a

#define LP8860_EEPROM_REG_0		0x60
#define LP8860_EEPROM_REG_1		0x61
#define LP8860_EEPROM_REG_2		0x62
#define LP8860_EEPROM_REG_3		0x63
#define LP8860_EEPROM_REG_4		0x64
#define LP8860_EEPROM_REG_5		0x65
#define LP8860_EEPROM_REG_6		0x66
#define LP8860_EEPROM_REG_7		0x67
#define LP8860_EEPROM_REG_8		0x68
#define LP8860_EEPROM_REG_9		0x69
#define LP8860_EEPROM_REG_10		0x6a
#define LP8860_EEPROM_REG_11		0x6b
#define LP8860_EEPROM_REG_12		0x6c
#define LP8860_EEPROM_REG_13		0x6d
#define LP8860_EEPROM_REG_14		0x6e
#define LP8860_EEPROM_REG_15		0x6f
#define LP8860_EEPROM_REG_16		0x70
#define LP8860_EEPROM_REG_17		0x71
#define LP8860_EEPROM_REG_18		0x72
#define LP8860_EEPROM_REG_19		0x73
#define LP8860_EEPROM_REG_20		0x74
#define LP8860_EEPROM_REG_21		0x75
#define LP8860_EEPROM_REG_22		0x76
#define LP8860_EEPROM_REG_23		0x77
#define LP8860_EEPROM_REG_24		0x78

#define LP8860_LOCK_EEPROM		0x00
#define LP8860_UNLOCK_EEPROM		0x01
#define LP8860_LOAD_EEPROM		0x01
#define LP8860_PROGRAM_EEPROM		0x02
#define LP8860_EEPROM_CODE_1		0x08
#define LP8860_EEPROM_CODE_2		0xba
#define LP8860_EEPROM_CODE_3		0xef

#define LP8860_CLEAR_FAULTS		0x01
#define LP8860_INVALID_ID       0x00

using namespace upm;

LP8860::LP8860(int gpioPower, int i2cBus)
{
    status = mraa::ERROR_INVALID_RESOURCE;
    mraa_set_log_level(7);
    pinPower = gpioPower;
    i2c = new mraa::I2c(i2cBus);
    i2c->address(LP8860_I2C_ADDR);
    if (isAvailable())
        status = mraa::SUCCESS;
   if (status != mraa::SUCCESS)
      UPM_THROW("i2c config failed.");
}

LP8860::~LP8860()
{
    delete i2c;
}


bool LP8860::isAvailable()
{
    bool wasPowered = true;

    // Check power state
    if(!isPowered()) {
        wasPowered = false;
        MraaUtils::setGpio(pinPower, 1);
    }

    // Read ID register
    // mraa_i2c_address(i2c, LP8860_I2C_ADDR);
    uint8_t id = i2c->readReg(LP8860_ID);
    // Turn off to save power if not required
    if (!wasPowered)
        MraaUtils::setGpio(pinPower, 0);

    return id >= 0x10;
}


bool LP8860::isPowered()
{
    return MraaUtils::getGpio(pinPower);
}


void LP8860::setPowerOn()
{
    if (!isPowered())
    {
        MraaUtils::setGpio(pinPower, 1);
        setBrightness(0);
        loadEEPROM();
        allowMaxCurrent();
    }
}


void LP8860::setPowerOff()
{
    MraaUtils::setGpio(pinPower, 0);
}


int LP8860::getBrightness()
{
    uint8_t msb = i2cReadByte(LP8860_DISP_CL1_BRT_MSB);
    uint8_t lsb = i2cReadByte(LP8860_DISP_CL1_BRT_LSB);
    int percent = (100 * ((int)msb << 8 | lsb)) / 0xFFFF;
    return percent;
}


void LP8860::setBrightness(int dutyPercent)
{
    int value = (0xFFFF * dutyPercent) / 100;
    int msb = value >> 8;
    int lsb = value & 0xFF;
    i2cWriteByte(LP8860_DISP_CL1_BRT_MSB, msb);
    i2cWriteByte(LP8860_DISP_CL1_BRT_LSB, lsb);
    value = (0x1FFF * dutyPercent) / 100;
    msb = value >> 8;
    lsb = value & 0xFF;
    i2cWriteByte(LP8860_CL2_BRT_MSB, msb);
    i2cWriteByte(LP8860_CL2_BRT_LSB, lsb);
    i2cWriteByte(LP8860_CL3_BRT_MSB, msb);
    i2cWriteByte(LP8860_CL3_BRT_LSB, lsb);
    i2cWriteByte(LP8860_CL4_BRT_MSB, msb);
    i2cWriteByte(LP8860_CL4_BRT_LSB, lsb);
}


void LP8860::loadEEPROM()
{
    const int eepromTableSize = 0x19;
    uint8_t eepromInitTable[] = {
        0xEF, 0xFF, 0xDC, 0xAE, 0x5F, 0xE5, 0xF2, 0x77,
        0x77, 0x71, 0x3F, 0xB7, 0x17, 0xEF, 0xB0, 0x87,
        0xCF, 0x72, 0xC5, 0xDE, 0x35, 0x06, 0xDE, 0xFF,
        0x3E
    };

    uint8_t* buf = new unsigned char[eepromTableSize + 1];

    // Load EEPROM
    // printf("Loading LP8860 EEPROM\n");
    i2cWriteByte(LP8860_EEPROM_CNTRL, LP8860_LOAD_EEPROM);
    usleep(100000);

    // Check contents and program if not already done
    i2cReadBuffer(LP8860_EEPROM_REG_0, buf, eepromTableSize);
    if (memcmp(eepromInitTable, buf, eepromTableSize) != 0)
    {
        printf("LP8860 EEPROM not initialized - programming...\n");
        // Unlock EEPROM
        i2cWriteByte(LP8860_EEPROM_UNLOCK, LP8860_EEPROM_CODE_1);
        i2cWriteByte(LP8860_EEPROM_UNLOCK, LP8860_EEPROM_CODE_2);
        i2cWriteByte(LP8860_EEPROM_UNLOCK, LP8860_EEPROM_CODE_3);
        i2cWriteBuffer(LP8860_EEPROM_REG_0, eepromInitTable, eepromTableSize);
        i2cWriteByte(LP8860_EEPROM_CNTRL, LP8860_PROGRAM_EEPROM);
        usleep(200000);
        i2cWriteByte(LP8860_EEPROM_CNTRL, 0);
        i2cWriteByte(LP8860_EEPROM_UNLOCK, LP8860_LOCK_EEPROM);
    }
}


void LP8860::allowMaxCurrent()
{
    i2cWriteByte(LP8860_DISP_CL1_CURR_MSB, 0x0F);
    i2cWriteByte(LP8860_DISP_CL1_CURR_LSB, 0xFF);
    i2cWriteByte(LP8860_CL2_CURRENT, 0xFF);
    i2cWriteByte(LP8860_CL3_CURRENT, 0xFF);
    i2cWriteByte(LP8860_CL4_CURRENT, 0xFF);
}


void LP8860::i2cWriteByte(int reg, int value)
{
    i2c->address(LP8860_I2C_ADDR);
    status = i2c->writeReg(static_cast<uint8_t>(reg), static_cast<uint8_t>(value));
    if (status != mraa::SUCCESS)
        UPM_THROW("i2cWriteByte failed");
}


uint8_t LP8860::i2cReadByte(uint8_t reg)
{
    uint8_t value;
    i2c->address(LP8860_I2C_ADDR);
    if (i2c->readBytesReg(reg, &value, 1) != 1)
        UPM_THROW("i2cReadByte failed");
    return value;
}


void LP8860::i2cWriteBuffer(int reg, uint8_t* buf, int length)
{
    if (length <= MAX_I2C_WRITE_SIZE)
    {
        uint8_t* writeBuf = new unsigned char[length + 1];
        writeBuf[0] = reg;
        memcpy(&writeBuf[1], buf, length);
        i2c->address(LP8860_I2C_ADDR);
        status = i2c->write(writeBuf, length + 1);
        delete[] writeBuf;
    }
    else
        status = mraa::ERROR_INVALID_PARAMETER;
    if (status != mraa::SUCCESS)
        UPM_THROW("i2cWriteBuffer failed");
}


void LP8860::i2cReadBuffer(int reg, uint8_t* buf, int length)
{
    i2c->address(LP8860_I2C_ADDR);
    if (i2c->readBytesReg(reg, buf, length) != length)
        UPM_THROW("i2cReadBuffer failed");
}

