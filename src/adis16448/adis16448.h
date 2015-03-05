////////////////////////////////////////////////////////////////////////////////////////////////////////
// March 2015
// By: Juan Jose Chong
////////////////////////////////////////////////////////////////////////////////////////////////////////
// ADIS16448.h
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
#include <string>
#include <mraa/spi.h>
#include <mraa/gpio.h>

// User Register Memory Map from Table 6 of the Datasheet
#define FLASH_CNT 0x00 //Flash memory write count
#define XGYRO_OUT 0x04
#define YGYRO_OUT 0x06 //Y-axis gyroscope output
#define ZGYRO_OUT 0x08 //Z-axis gyroscope output
#define XACCL_OUT 0x0A //X-axis accelerometer output
#define YACCL_OUT 0x0C //Y-axis accelerometer output
#define ZACCL_OUT 0x0E //Z-axis accelerometer output
#define XMAGN_OUT 0X10 //X-axis magnetometer output
#define YMAGN_OUT 0x12 //Y-axis magnetometer output
#define ZMAGN_OUT 0x14 //Z-axis magnetometer output
#define BARO_OUT 0x16 //Barometer pressure measurement, high word
#define TEMP_OUT 0x18 //Temperature output
#define XGYRO_OFF 0x1A //X-axis gyroscope bias offset factor
#define YGYRO_OFF 0x1C //Y-axis gyroscope bias offset factor
#define ZGYRO_OFF 0x1E //Z-axis gyroscope bias offset factor
#define XACCL_OFF 0x20 //X-axis acceleration bias offset factor
#define YACCL_OFF 0x22 //Y-axis acceleration bias offset factor
#define ZACCL_OFF 0x24 //Z-axis acceleration bias offset factor
#define XMAGN_HIC 0x26 //X-axis magnetometer, hard iron factor
#define YMAGN_HIC 0x28 //Y-axis magnetometer, hard iron factor
#define ZMAGN_HIC 0x2A //Z-axis magnetometer, hard iron factor
#define XMAGN_SIC 0x2C //X-axis magnetometer, soft iron factor
#define YMAGN_SIC 0x2E //Y-axis magnetometer, soft iron factor
#define ZMAGN_SIC 0x30 //Z-axis magnetometer, soft iron factor
#define GPIO_CTRL 0x32 //GPIO control
#define MSC_CTRL 0x34 //Misc. control
#define SMPL_PRD 0x36 //Sample clock/Decimation filter control
#define SENS_AVG 0x38 //Digital filter control
#define SEQ_CNT 0x3A //xMAGN_OUT and BARO_OUT counter
#define DIAG_STAT 0x3C //System status
#define GLOB_CMD 0x3E //System command
#define ALM_MAG1 0x40 //Alarm 1 amplitude threshold
#define ALM_MAG2 0x42 //Alarm 2 amplitude threshold
#define ALM_SMPL1 0x44 //Alarm 1 sample size
#define ALM_SMPL2 0x46 //Alarm 2 sample size
#define ALM_CTRL 0x48 //Alarm control
#define LOT_ID1 0x52 //Lot identification number
#define LOT_ID2 0x54 //Lot identification number
#define PROD_ID 0x56 //Product identifier
#define SERIAL_NUM 0x58 //Lot-specific serial number

namespace upm {

	// ADIS16448 class definition
	class ADIS16448{

		public:

		// Constructor with configurable CS, data ready, and HW reset pins
		ADIS16448(int bus, int rst);

		//Destructor
		~ADIS16448();

		//Performs hardware reset by sending pin 8 low on the DUT for 2 seconds
		void resetDUT();

		//Sets SPI bit order, clock divider, and data mode
		void configSPI();

		//Read sensor
		int16_t regRead(uint8_t regAddr);

		//Write register
		void regWrite(uint8_t regAddr, uint16_t regData);

		//Scale accelerator data
		float accelScale(int16_t sensorData);

		//Scale gyro data
		float gyroScale(int16_t sensorData);

		//Scale temperature data
		float tempScale(int16_t sensorData);

		//Scale pressure data
		float pressureScale(int16_t sensorData);

		//Scale magnetometer data
		float magnetometerScale(int16_t sensorData);

		private:

		mraa_spi_context _spi;
		mraa_gpio_context _rst;

	};
}

