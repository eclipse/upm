/*
 * Author: Marc Graham <marc@m2ag.net>
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
#include "ads1115.hpp"

using namespace upm;

ADS1115::ADS1115(int bus, uint8_t address) : ADS1X15(bus, address) {
    m_name = "ADS1115";
    m_conversionDelay = ADS1115_CONVERSIONDELAY;
    m_bitShift = 0;
    ADS1X15::getCurrentConfig();
}

ADS1115::~ADS1115(){};

void
ADS1115::setSPS(ADSDATARATE rate){
    updateConfigRegister((m_config_reg & ~ADS1X15_DR_MASK) | rate);
}

//Protected functions
float
ADS1115::getMultiplier(void){
    float multi = 0.0;
    switch((ADSGAIN)m_config_reg & ADS1X15_PGA_MASK){
    case GAIN_TWOTHIRDS:
        multi = 0.0001875;
        break;
    case GAIN_ONE:
        multi = 0.000125;
        break;
    case GAIN_TWO:
        multi = 0.0000625;
        break;
    case GAIN_FOUR:
        multi = 0.00003125;
        break;
    case GAIN_EIGHT:
        multi = 0.000015625;
        break;
    case GAIN_SIXTEEN:
        multi = 0.0000078125;
        break;
    default:
        multi = 0.0001875;
        break;
    }
    return multi;
}

void
ADS1115::setDelay(){
    switch((int)ADS1X15::getSPS()){
    case 0:
        m_conversionDelay = 126000;
        break;
    case 32:
        m_conversionDelay = 63000;
        break;
    case 64:
        m_conversionDelay = 32000;
        break;
    case 96:
        m_conversionDelay = 16000;
        break;
    case 128:
        m_conversionDelay = 8000;
        break;
    case 160:
        m_conversionDelay = 4500;
        break;
    case 192:
        m_conversionDelay = 2200;
        break;
    case 224:
        m_conversionDelay = 1200;
        break;
    default:
        m_conversionDelay = 126000;
        break;
    }
}
