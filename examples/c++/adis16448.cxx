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
// This example code runs on an Intel Edison and uses mraa to acquire data
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
#include <unistd.h>
#include <iostream>
#include <signal.h>

#include "adis16448.h"

int
main(int argc, char **argv)
{
    while(true)
    {
    //! [Interesting]
        upm::ADIS16448* imu = new upm::ADIS16448(0,3); //upm::ADIS16448(SPI,RST)

        //Read the specified register, scale it, and display it on the screen
        std::cout << "XGYRO_OUT:" << imu->gyroScale(imu->regRead(XGYRO_OUT)) << std::endl;
        std::cout << "YGYRO_OUT:" << imu->gyroScale(imu->regRead(YGYRO_OUT)) << std::endl;
        std::cout << "ZGYRO_OUT:" << imu->gyroScale(imu->regRead(ZGYRO_OUT)) << std::endl;
        std::cout << " " << std::endl;
        std::cout << "XACCL_OUT:" << imu->accelScale(imu->regRead(XACCL_OUT)) << std::endl;
        std::cout << "YACCL_OUT:" << imu->accelScale(imu->regRead(YACCL_OUT)) << std::endl;
        std::cout << "ZACCL_OUT:" << imu->accelScale(imu->regRead(ZACCL_OUT)) << std::endl;
        std::cout << " " << std::endl;
    //! [Interesting]
        sleep(1);
    }
    return (0);
}
