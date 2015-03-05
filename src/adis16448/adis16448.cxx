////////////////////////////////////////////////////////////////////////////////////////////////////////
// March 2015
// By: Juan Jose Chong
////////////////////////////////////////////////////////////////////////////////////////////////////////
// ADIS16448.cxx
////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This file interfaces the ADIS16448 IMU with an Intel Edison. 
//
// This example is free software. You can redistribute it and/or modify it
// under the terms of the GNU Lesser Public License as published by the Free Software
// Foundation, either version 3 of the License, or any later version.
//
// This example is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser Public License for more details.
//
// You should have received a copy of the GNU Lesser Public License along with
// this example. If not, see <http://www.gnu.org/licenses/>.
//
// This library is based on the ADIS16480 library written by Daniel Tatum.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
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
	_rst = mraa_gpio_init(rst); //Initialize RST pin
	mraa_gpio_dir(_rst, MRAA_GPIO_IN); //Set direction as INPUT

// Configure SPI
	_spi = mraa_spi_init(bus);
	mraa_spi_frequency(_spi, 1000000); //Set SPI frequency to 1MHz
	mraa_spi_mode(_spi, MRAA_SPI_MODE3); //Set SPI mode/polarity
	mraa_spi_bit_per_word(_spi, 16); //Set # of bits per word
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
	mraa_spi_mode(_spi, MRAA_SPI_MODE3); //Set SPI mode/polarity
	mraa_spi_bit_per_word(_spi, 16); //Set # of bits per word
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
