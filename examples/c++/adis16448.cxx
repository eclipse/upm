////////////////////////////////////////////////////////////////////////////////////////////////////////
// January 2015
// By: Juan Jose Chong
////////////////////////////////////////////////////////////////////////////////////////////////////////
// ADIS16448.css
////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This example interfaces with an ADIS16448 using SPI, reads IMU data, scales, and outputs
/// data to the screen.
//
// This project has been tested on an Intel Edison using the Intel Breakout Board and a
// TI TXB0104 level shifter. 
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
#include <unistd.h>
#include <iostream>
#include <signal.h>

#include "adis16448.h"

int
main(int argc, char **argv)
{
	while(true)
	{
		upm::ADIS16448* imu = new upm::ADIS16448(0,3); //SPI,RST

		//std::cout << "\r" << "XGYRO_OUT:" << imu->accelScale(imu->regRead(XGYRO_OUT));

		std::cout << "XGYRO_OUT:" << imu->gyroScale(imu->regRead(XGYRO_OUT)) << std::endl;
		std::cout << "YGYRO_OUT:" << imu->gyroScale(imu->regRead(YGYRO_OUT)) << std::endl;
		std::cout << "ZGYRO_OUT:" << imu->gyroScale(imu->regRead(ZGYRO_OUT)) << std::endl;
		std::cout << " " << std::endl;
		std::cout << "XACCL_OUT:" << imu->accelScale(imu->regRead(XACCL_OUT)) << std::endl;
		std::cout << "YACCL_OUT:" << imu->accelScale(imu->regRead(YACCL_OUT)) << std::endl;
		std::cout << "ZACCL_OUT:" << imu->accelScale(imu->regRead(ZACCL_OUT)) << std::endl;
		std::cout << " " << std::endl;

		sleep(1);
	}
	return (0);
}
