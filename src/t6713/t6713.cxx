/*
 * Author: HCL
 * Copyright (c) 2015 Intel Corporation.
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
#include <math.h>
#include <errno.h>
#include <t6713.h>

#define T6713_ADDR 0x15

/* REGISTER ADDRESSES */
#define T6713_REG_FIRMWARE_REVISION_MSB               0x13
#define T6713_REG_FIRMWARE_REVISION_LSB               0x89

#define T6713_REG_STATUS_MSB                          0x13
#define T6713_REG_STATUS_LSB                          0x8A

#define T6713_REG_GAS_PPM_MSB                         0x13
#define T6713_REG_GAS_PPM_LSB                         0x8B

#define T6713_REG_RESET_DEVICE_MSB                    0x03
#define T6713_REG_RESET_DEVICE_LSB                    0xE8

#define T6713_REG_START_SINGLE_POINT_CAL_MSB          0x03
#define T6713_REG_START_SINGLE_POINT_CAL_LSB          0xEC

#define T6713_REG_CHANGE_SLAVE_ADDRESS_MSB            0x0F
#define T6713_REG_CHANGE_SLAVE_ADDRESS_LSB            0xA5

#define T6713_REG_ABC_LOGIC_ENABLE_DISABLE_MSB        0x03
#define T6713_REG_ABC_LOGIC_ENABLE_DISABLE_LSB        0xEE

using namespace upm;
using namespace upm::t6713_co2;

    T6713::T6713 (int bus)
    {
        i2c = new mraa::I2c(bus);
        status = i2c->address(T6713_ADDR);
    if (!isConfigured())
        UPM_THROW("config failure");
    }

    T6713::~T6713()
    {
        delete i2c;
    }

    bool T6713::isConfigured()
    {
        return status == mraa::SUCCESS;
    }

    const char* T6713::getModuleName() 
    { 
        return "t6713"; 
    }          

    uint16_t T6713::getPpm ()
    {
	RESPONSE * response  = new RESPONSE ;
	uint16_t data , currStatus;

	switch(currStatus = getStatus())  /* handle error conditions */
	{
		case ERROR_CONDITION:
		     printf("\nerror condition");
		break;
		case FLASH_ERROR:
		     printf("\nflash error");
		break;
		case CALIBRATION_ERROR:
		     printf("\ncalibration error ");
		break;
		case WARMUP_MODE:
		     printf("\nwarm up mode");
		break;
		case RS232:
		     printf("\nRS232 mode set ");
		break;
		case RS485:
		     printf("\nRS485 mode set ");
		break;
		case I2C:
		     printf("\nI2C mode set");
		     data = 0;
	             runCommand(T6713_COMMAND_GET_GAS_PPM);
	  	     sleepMs(50); /*check for if delay is required */
	             i2c->read((uint8_t*)(response), sizeof(RESPONSE) );
	             if(response->function_code == READ_INPUT_REGISTERS)
	             {
			if(response->byte_count == 2)
			{
				data = (response->status_msb << 8 | response->status_lsb);
			}
		     }
		     delete(response);
	             return(data);
		break;
	}		
	return 0;

    }

    uint16_t T6713::getFirmwareRevision()
    {
	//return(sendCommand(T6713_COMMAND_GET_FIRMWARE_REVISION));
    }


    mraa::Result T6713::runCommand(MODBUS_COMMANDS cmd) {

    COMMAND * cmdPacket = new COMMAND ;
    printf("\nrunning modbus command for gas sensor\n");
	switch(cmd)
	{
	case T6713_COMMAND_RESET:
		cmdPacket->function_code = WRITE_SINGLE_COIL;
        	cmdPacket->register_address_msb = T6713_REG_RESET_DEVICE_MSB;
        	cmdPacket->register_address_lsb = T6713_REG_RESET_DEVICE_LSB;
        	cmdPacket->input_registers_to_read_msb = 0xff;
        	cmdPacket->input_registers_to_read_lsb = 0x00;
  		status = i2c->write((const uint8_t*) (cmdPacket), sizeof(COMMAND));
	     /*no response from the slave */
	break;
	case T6713_COMMAND_STATUS:
		/*created the modbus status command packet*/
		cmdPacket->function_code = READ_INPUT_REGISTERS;
		cmdPacket->register_address_msb = T6713_REG_STATUS_MSB;
		cmdPacket->register_address_lsb = T6713_REG_STATUS_LSB;
		cmdPacket->input_registers_to_read_msb = 0;
		cmdPacket->input_registers_to_read_lsb = 1;

		status = i2c->write((const uint8_t*) (cmdPacket), sizeof(COMMAND));
	break;
	case T6713_COMMAND_GET_FIRMWARE_REVISION:
		cmdPacket->function_code = READ_INPUT_REGISTERS;
		cmdPacket->register_address_msb = T6713_REG_FIRMWARE_REVISION_MSB;
		cmdPacket->register_address_lsb = T6713_REG_FIRMWARE_REVISION_LSB;
		cmdPacket->input_registers_to_read_msb = 0;
		cmdPacket->input_registers_to_read_lsb = 1;
		status = i2c->write((const uint8_t*) (cmdPacket), sizeof(COMMAND));
	break;
	case T6713_COMMAND_GET_GAS_PPM:
		cmdPacket->function_code = READ_INPUT_REGISTERS;
		cmdPacket->register_address_msb = T6713_REG_GAS_PPM_MSB;
		cmdPacket->register_address_lsb = T6713_REG_GAS_PPM_LSB;

		cmdPacket->input_registers_to_read_msb = 0;
		cmdPacket->input_registers_to_read_lsb = 1;

		status = i2c->write((const uint8_t*) (cmdPacket), sizeof(COMMAND));
	break;
	//default:
	}
	delete cmdPacket;
	if (status != mraa::SUCCESS)
	return status;


}


STATUS T6713::getStatus() {
	uint16_t status ;
	RESPONSE * response  = new RESPONSE ;
	runCommand(T6713_COMMAND_STATUS);
	sleepMs(50); /*check for if delay is required */
	i2c->read((uint8_t*)(response), sizeof(RESPONSE) );
	if(response->function_code == READ_INPUT_REGISTERS)
	{
		if(response->byte_count == 2)
		{
			status = (response->status_msb << 8 | response->status_lsb);
		}
	}
	delete(response);

	if(status & 0x0001)
	{
		return ERROR_CONDITION;
	}
	if(status & 0x0002)
	{
		return FLASH_ERROR;
	}
	if(status & 0x0004)
	{
		return CALIBRATION_ERROR;
	}
	if(status & 0x0800)
	{
		return WARMUP_MODE;
	}
	if(status & 0x8000)
	{
		return SINGLE_POINT_CALIBRATION;
	}
	if(status & 0x0100)
	{
		return RS232;
	}
	if(status & 0x0200)
	{
		return I2C;
	}
	if(status & 0x0400)
	{
		return RS485;
	}
}

void T6713::sleepMs(int mseconds)
{
    struct timespec sleepTime;

    sleepTime.tv_sec = mseconds / 1000; // Number of seconds
    sleepTime.tv_nsec = ( mseconds % 1000 ) * 1000000; // Convert fractional seconds to nanoseconds

    // Iterate nanosleep in a loop until the total sleep time is the original
    // value of the seconds parameter
    while ( ( nanosleep( &sleepTime, &sleepTime ) != 0 ) && ( errno == EINTR ) );
}


