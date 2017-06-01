/*
    MutichannelGasSensor.h
    2015 Copyright (c) Seeed Technology Inc.  All right reserved.

    Author: Jacky Zhang
    2015-3-17
    http://www.seeed.cc/
    modi by Jack, 2015-8
    
    Migrated to mraa / upm libraries to be used with  IoT INTEL Edison or other
    modi by G. Vidal ENS de Lyon 2016-03 http://ife.ens-lyon.fr
  
    The MIT License (MIT)

    Copyright (c) 2015 Seeed Technology Inc.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/
#pragma once


#include <iostream>
#include <string>
#include "mraa.hpp"
#include "mraa/i2c.hpp"



#define DEFAULT_I2C_ADDR    0x04

namespace upm {
     enum gas {CO, NO2, NH3, C3H8, C4H10, CH4, H2, C2H5OH};

class MICS6814 {

private:

    mraa::I2c* i2c;
    std::string m_name;
    uint8_t m_address;//I2C address of this MCU
    uint16_t res0[3];//sensors res0
    uint16_t res[3];//sensors res
    uint8_t is_connected;
    void sendI2C(uint8_t dta);
    int16_t readData(uint8_t cmd);
    int16_t readR0(void);
    int16_t readR(void);
    float calcGas(int gas);

public:

     /**
     * MICS6814 constructor
     *
     * @param bus i2c bus the sensor is attached to.
     * @param address. Device address. Default is 0x70.
     */
     MICS6814 (int bus, uint8_t address = 0x04);

     /**
     * MICS6814 destructor
     */
     ~MICS6814 ();

    void changeI2cAddr(uint8_t newAddr);
    void powerOn(void);
    void powerOff(void);
    void doCalibrate(void);
    //get gas concentration, unit: ppm
    float measure_CO();
    float measure_NO2();
    float measure_NH3();
    float measure_C3H8();
    float measure_C4H10();
    float measure_CH4();
    float measure_H2();
    float measure_C2H5OH();

};


}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
