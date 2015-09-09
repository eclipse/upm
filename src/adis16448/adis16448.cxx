//////////////////////////////////////////////////////////////////////////////////////
// The MIT License (MIT)
//
// Submit Date: 03/09/2015
// Author: Juan Jose Chong <juanjchong@gmail.com>
// Copyright (c) 2015 Juan Jose Chong
//
//////////////////////////////////////////////////////////////////////////////////////
// adis16448.cxx
//////////////////////////////////////////////////////////////////////////////////////
//
// This library runs on an Intel Edison and uses mraa to acquire data
// from an ADIS16448. This data is then scaled and printed onto the terminal.
// 
// This software has been tested to connect to an ADIS16448 through a level shifter
// such as the TI TXB0104. The SPI lines (DIN, DOUT, SCLK, /CS) are all wired through
// the level shifter and the ADIS16448 is also being powered by the Intel Edison.
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <stdexcept>
#include <unistd.h>
#include <stdlib.h>
#include <functional>
#include <string.h>

#include "adis16448.h"

using namespace upm;

////////////////////////////////////////////////////////////////////////////
// Constructor with configurable CS, DR, and RST
////////////////////////////////////////////////////////////////////////////
// RST - Hardware reset pin
////////////////////////////////////////////////////////////////////////////
ADIS16448::ADIS16448(int bus, int rst)
{
// Configure I/O
        //Initialize RST pin
        if ( !(_rst = mraa_gpio_init(rst)) ) 
          {
            throw std::invalid_argument(std::string(__FUNCTION__) +
                                        ": mraa_gpio_init() failed, invalid pin?");
            return;
          }
	mraa_gpio_dir(_rst, MRAA_GPIO_IN); //Set direction as INPUT

        // Configure SPI
        if ( !(_spi = mraa_spi_init(bus)) ) 
          {
            throw std::invalid_argument(std::string(__FUNCTION__) +
                                        ": mraa_spi_init() failed");
            return;
          }
        configSPI();
}

////////////////////////////////////////////////////////////////////////////
// Destructor - Stops SPI and Closes all GPIO used. Reports an error if
// unable to close either properly.
////////////////////////////////////////////////////////////////////////////
ADIS16448::~ADIS16448()
{
// Close SPI bus
	mraa_result_t error;
	error = mraa_spi_stop(_spi);
	if(error != MRAA_SUCCESS)
	{
		mraa_result_print(error);
	}
// Close GPIO
	error = mraa_gpio_close(_rst);
	if(error != MRAA_SUCCESS)
	{
		mraa_result_print(error);
	}
}

////////////////////////////////////////////////////////////////////////////
// Performs hardware reset by setting _RST pin low for 2 seconds.
////////////////////////////////////////////////////////////////////////////
void ADIS16448::resetDUT()
{
	mraa_gpio_write(_rst, 0);
	usleep(100000); //Sleep for 100ms
	mraa_gpio_write(_rst, 1);
	usleep(1000000); //Sleep for 1s
}

////////////////////////////////////////////////////////////////////////////
// Sets SPI bit order, clock divider, and data mode. This function is useful
// when there are multiple SPI devices using different settings.
////////////////////////////////////////////////////////////////////////////
void ADIS16448::configSPI() {
	mraa_spi_frequency(_spi, 1000000); //Set SPI frequency to 1MHz

        if ( mraa_spi_mode(_spi, MRAA_SPI_MODE3) != MRAA_SUCCESS ) 
          {
            throw std::invalid_argument(std::string(__FUNCTION__) +
                                        ": mraa_spi_mode() failed");
            return;
          }
	//Set # of bits per word

        if ( mraa_spi_bit_per_word(_spi, 16) != MRAA_SUCCESS ) 
          {
            throw std::invalid_argument(std::string(__FUNCTION__) +
                                        ": mraa_spi_bit_per_word() failed");
            return;
          }
}

////////////////////////////////////////////////////////////////////////////////////////////
// Reads two bytes (one word) in two sequential registers over SPI
////////////////////////////////////////////////////////////////////////////////////////////
// regAddr - register address from the lookup table in ADIS16448.h
// return - (int) signed 16 bit 2's complement number
////////////////////////////////////////////////////////////////////////////////////////////
int16_t ADIS16448::regRead(uint8_t regAddr)
{
	configSPI(); //Set up SPI (useful when multiple SPI devices present on bus)
// Write register address to be read
	uint8_t buf[2]; //Allocate write buffer
	memset(buf, 0, sizeof(uint8_t)*2); //Initialize buffer and write 0s
	buf[1] = regAddr; //Write the user-requested register address to the buffer
	mraa_spi_write_buf(_spi, buf, 2); //Write the buffer onto the SPI port

	usleep(20); //Delay to not violate read rate (210us)

// Read data from register requested
	buf[1] = 0; //Clear contents of write buffer
	uint8_t* x = mraa_spi_write_buf(_spi, buf, 2); //Write 0x0000 to SPI and read data requested above
	int16_t _dataOut = (x[1] << 8) | (x[0] & 0xFF);; //Concatenate upper and lower bytes

	usleep(20); //delay to not violate read rate (210us)
	return(_dataOut);
}
////////////////////////////////////////////////////////////////////////////
// Writes one byte of data to the specified register over SPI
////////////////////////////////////////////////////////////////////////////
// regAddr - register address from the lookup table
// regData - data to be written to the register
////////////////////////////////////////////////////////////////////////////
void ADIS16448::regWrite(uint8_t regAddr,uint16_t regData)
{
	configSPI();
// Separate the 16 bit command word into two bytes
	uint16_t addr = (((regAddr & 0x7F) | 0x80) << 8); //Check that the address is 7 bits, flip the sign bit
	uint16_t lowWord = (addr | (regData & 0xFF));
	uint16_t highWord = ((addr | 0x100) | ((regData >> 8) & 0xFF));
// Write the low byte to the SPI bus
	uint8_t lbuf[2]; //Allocate write buffer
	memset(lbuf, 0, sizeof(uint8_t)*2); //Fill low buffer with 0's
	lbuf[0] = (lowWord >> 8);
	lbuf[1] = (lowWord);
	mraa_spi_write_buf(_spi, lbuf, 2); //Write the buffer to the SPI port

	usleep(20);

// Write the high byte to the SPI bus
	uint8_t hbuf[2]; //Allocate write buffer
	memset(hbuf, 0, sizeof(uint8_t)*2); //Fill high buffer with 0's
	hbuf[0] = (highWord >> 8);
	hbuf[1] = (highWord);
	mraa_spi_write_buf(_spi, hbuf, 2); //Write the buffer to the SPI port

	usleep(20);

}
/////////////////////////////////////////////////////////////////////////////////////////
// Converts accelerometer data output from the sensorRead() function and returns
// acceleration in g's
/////////////////////////////////////////////////////////////////////////////////////////
// sensorData - data output from sensorRead()
// return - (float) signed/scaled accelerometer in G's
/////////////////////////////////////////////////////////////////////////////////////////
float ADIS16448::accelScale(int16_t sensorData)
{
	float finalData = sensorData * 0.000833; // multiply by accel sensitivity (250uG/LSB)
	return finalData;
}
/////////////////////////////////////////////////////////////////////////////////////////////
// Converts gyro data output from the sensorRead() function and returns gyro rate in deg/sec
/////////////////////////////////////////////////////////////////////////////////////////////
// sensorData - data output from sensorRead()
// return - (float) signed/scaled gyro in degrees/sec
/////////////////////////////////////////////////////////////////////////////////////////
float ADIS16448::gyroScale(int16_t sensorData)
{
	float finalData = sensorData * 0.04; //multiply by gyro sensitivity (0.005 LSB/dps)
	return finalData;
}
/////////////////////////////////////////////////////////////////////////////////////////////
// Converts temperature data output from the sensorRead() function and returns temperature
// in degrees Celcius
/////////////////////////////////////////////////////////////////////////////////////////////
// sensorData - data output from sensorRead()
// return - (float) signed/scaled temperature in degrees Celcius
/////////////////////////////////////////////////////////////////////////////////////////
float ADIS16448::tempScale(int16_t sensorData)
{
	float finalData = (sensorData * 0.07386) + 31; //multiply by temperature scale and add 31 to equal 0x0000
	return finalData;
}
/////////////////////////////////////////////////////////////////////////////////////////////
// Converts barometer data output from sensorRead() function and returns pressure in bar
/////////////////////////////////////////////////////////////////////////////////////////////
// sensorData - data output from sensorRead()
// return - (float) signed/scaled temperature in degrees Celcius
/////////////////////////////////////////////////////////////////////////////////////////
float ADIS16448::pressureScale(int16_t sensorData)
{
	float finalData = (sensorData * 0.02); //multiply by gyro sensitivity (0.005 LSB/dps)
	return finalData;
}
/////////////////////////////////////////////////////////////////////////////////////////////
// Converts magnetometer output from sensorRead() function and returns magnetic field
// reading in Gauss
/////////////////////////////////////////////////////////////////////////////////////////////
// sensorData - data output from sensorRead()
// return - (float) signed/scaled temperature in degrees Celcius
/////////////////////////////////////////////////////////////////////////////////////////
float ADIS16448::magnetometerScale(int16_t sensorData)
{
	float finalData = (sensorData * 0.0001429); //multiply by sensor resolution (142.9uGa LSB/dps)
	return finalData;
}
