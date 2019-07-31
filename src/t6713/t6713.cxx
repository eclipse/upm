/*
 * Author: Henry Bruce <henry.bruce@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <errno.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <syslog.h>
#include <unistd.h>

#include "t6713.hpp"

#define T6713_ADDR                                0x15

/* REGISTER ADDRESSES */
#define T6713_REG_FIRMWARE_REVISION               0x1389

#define T6713_REG_STATUS                          0x138A

#define T6713_REG_GAS_PPM                         0x138B

#define T6713_REG_RESET_DEVICE                    0x03E8

#define T6713_REG_START_SINGLE_POINT_CAL          0x03EC

#define T6713_REG_CHANGE_SLAVE_ADDRESS            0x0FA5

#define T6713_REG_ABC_LOGIC_ENABLE_DISABLE        0x03EE

using namespace upm;
using namespace upm::t6713_co2;

T6713::T6713 (int bus) : i2c(bus)
{
    status = i2c.address(T6713_ADDR);
    uint16_t firmwareRevision = getFirmwareRevision();
    if (firmwareRevision != mraa::SUCCESS)
        UPM_THROW("config failure");
}

uint16_t T6713::getFirmwareRevision()
{
    return(getSensorData(T6713_COMMAND_GET_FIRMWARE_REVISION));
}

uint16_t T6713::getPpm ()
{
    return(getSensorData(T6713_COMMAND_GET_GAS_PPM));
}

float T6713::getConcentration ()
{
    return(getSensorData(T6713_COMMAND_GET_GAS_PPM));
}

uint16_t T6713::getSensorData (MODBUS_COMMANDS cmd)
{
    uint16_t data ,readBytes ;
    STATUS currStatus ;
    switch(currStatus = getStatus()) /* handle error conditions */
    {
        case ERROR_CONDITION:
            UPM_THROW ("error condition");
            break;
        case FLASH_ERROR:
            UPM_THROW ("flash error");
            break;
        case CALIBRATION_ERROR:
            UPM_THROW ("calibration error");
            break;
        case WARMUP_MODE:
            //UPM_THROW ("warmup mode");
            break;
        case RS232:
            //printf("\nRS232 mode set\n ");
            break;
        case RS485:
            //printf("\nRS485 mode set\n ");
            break;
        case I2C:
            {
                //printf("\nI2C mode set\n");
                data = 0;
                runCommand(cmd);
                RESPONSE response;
                if((readBytes = i2c.read((uint8_t*)(&response), sizeof(RESPONSE) ) != sizeof(RESPONSE)))
                {
                    UPM_THROW("I2C read failed");
                    // TODO
                }
                if(response.function_code == READ_INPUT_REGISTERS)
                {
                    if(response.byte_count == 2)
                    {
                        data = (response.status_msb << 8 | response.status_lsb);
                    }
                }
                return(data);
                break;
            }
        default:
            syslog(LOG_WARNING, "%s: switch case not defined",
                    std::string(__FUNCTION__).c_str());
    }
    return 0;

}


mraa::Result T6713::runCommand(MODBUS_COMMANDS cmd)
{
    COMMAND cmdPacket;
    mraa::Result ret = mraa::SUCCESS;

    switch(cmd)
    {
        case T6713_COMMAND_RESET:
            cmdPacket.function_code = WRITE_SINGLE_COIL;
            cmdPacket.register_address_msb = (T6713_REG_RESET_DEVICE >> 8);
            cmdPacket.register_address_lsb = (T6713_REG_RESET_DEVICE & 0xff);
            cmdPacket.input_registers_to_read_msb = 0xff;
            cmdPacket.input_registers_to_read_lsb = 0x00;
            ret = i2c.write((const uint8_t*) (&cmdPacket), sizeof(COMMAND));
            /*no response from the slave */
            break;
        case T6713_COMMAND_STATUS:
            /*created the modbus status command packet*/
            cmdPacket.function_code = READ_INPUT_REGISTERS;
            cmdPacket.register_address_msb = (T6713_REG_STATUS >> 8);
            cmdPacket.register_address_lsb = (T6713_REG_STATUS & 0xff);
            cmdPacket.input_registers_to_read_msb = 0;
            cmdPacket.input_registers_to_read_lsb = 1;

            if((ret = i2c.write((const uint8_t*) (&cmdPacket), sizeof(COMMAND))) != mraa::SUCCESS)
            {
                UPM_THROW("I2C write failed");
            }


            break;
        case T6713_COMMAND_GET_FIRMWARE_REVISION:
            cmdPacket.function_code = READ_INPUT_REGISTERS;
            cmdPacket.register_address_msb = (T6713_REG_FIRMWARE_REVISION >> 8);
            cmdPacket.register_address_lsb = (T6713_REG_FIRMWARE_REVISION & 0xff);
            cmdPacket.input_registers_to_read_msb = 0;
            cmdPacket.input_registers_to_read_lsb = 1;
            ret = i2c.write((const uint8_t*) (&cmdPacket), sizeof(COMMAND));
            break;
        case T6713_COMMAND_GET_GAS_PPM:
            cmdPacket.function_code = READ_INPUT_REGISTERS;
            cmdPacket.register_address_msb = (T6713_REG_GAS_PPM >> 8);
            cmdPacket.register_address_lsb = (T6713_REG_GAS_PPM & 0xff);

            cmdPacket.input_registers_to_read_msb = 0;
            cmdPacket.input_registers_to_read_lsb = 1;

            if((ret = i2c.write((const uint8_t*) (&cmdPacket), sizeof(COMMAND))) != mraa::SUCCESS)
            {
                UPM_THROW("I2C write failed");
            }

            break;
    }

    return ret;
}


STATUS T6713::getStatus()
{
    uint16_t responseStatus = 0, readBytes = 0;
    RESPONSE response;
    runCommand(T6713_COMMAND_STATUS);
    if((readBytes = i2c.read((uint8_t*) (&response), sizeof(RESPONSE)) != sizeof(RESPONSE)))
    {
        UPM_THROW("I2C read failed");

    }
    if(response.function_code == READ_INPUT_REGISTERS)
    {
        if(response.byte_count == 2)
        {
            responseStatus = (response.status_msb << 8 | response.status_lsb);
        }
        else
        {
            UPM_THROW("I2C read failed");

        }
    }
    else
    {
        UPM_THROW("MODBUS function code failed");
    }

    if(responseStatus & 0x0001)
    {
        return ERROR_CONDITION;
    }
    if(responseStatus & 0x0002)
    {
        return FLASH_ERROR;
    }
    if(responseStatus & 0x0004)
    {
        return CALIBRATION_ERROR;
    }
    if(responseStatus & 0x0800)
    {
        return WARMUP_MODE;
    }
    if(responseStatus & 0x8000)
    {
        return SINGLE_POINT_CALIBRATION;
    }
    if(responseStatus & 0x0100)
    {
        return RS232;
    }
    if(responseStatus & 0x0400)
    {
        return RS485;
    }
    else
    {
        return I2C;
    }
}
