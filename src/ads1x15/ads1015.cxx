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
#include "ads1015.hpp"

using namespace upm;

void
ADS1015::setSPS(ADSSAMPLERATE rate){
    updateConfigRegister((m_config_reg & ~ADS1X15_DR_MASK) | rate);
}

ADS1015::ADS1015(int bus, uint8_t address, float vref) : ADS1X15(bus, address) {
    m_name = "ADS1015";
    m_conversionDelay = ADS1015_CONVERSIONDELAY;
    m_bitShift = 4;
    ADS1X15::getCurrentConfig();
    if (vref < 0.0 || vref > 6.144)
        UPM_THROW("vref out of range");
    else if (vref > 4.096)
        setGain(GAIN_TWOTHIRDS);
    else if (vref > 2.048)
        setGain(GAIN_ONE);
    else if (vref > 1.024)
        setGain(GAIN_TWO);
    else if (vref > 0.512)
        setGain(GAIN_FOUR);
    else if (vref > 0.256)
        setGain(GAIN_EIGHT);
    else
        setGain(GAIN_SIXTEEN);
}

ADS1015::~ADS1015(){};

const char*
ADS1015::getModuleName() {
    return m_name.c_str();
}


unsigned int
ADS1015::getNumInputs() {
    return 4;
}

unsigned int
ADS1015::getResolutionInBits() {
    return 12;
}

uint16_t
ADS1015::getRawValue(unsigned int input) {
    ADS1X15::ADSMUXMODE mode = getMuxMode(input);
    updateConfigRegister((m_config_reg & ~ADS1X15_MUX_MASK) | mode, true);
    usleep(m_conversionDelay);
    uint16_t value = i2c->readWordReg(ADS1X15_REG_POINTER_CONVERT);
    value = value >> m_bitShift;
    return swapWord(value);
}

float
ADS1015::getVoltage(unsigned int input) {
    ADSMUXMODE mode = getMuxMode(input);
    return getSample(mode);
}



//Private functions
float
ADS1015::getMultiplier(void){
    float multi = 0.0;
    switch((ADSGAIN)m_config_reg & ADS1X15_PGA_MASK){
    case GAIN_TWOTHIRDS:
        multi = 0.003;
        break;
    case GAIN_ONE:
        multi = 0.002;
        break;
    case GAIN_TWO:
        multi = 0.001;
        break;
    case GAIN_FOUR:
        multi = 0.0005;
        break;
    case GAIN_EIGHT:
        multi = 0.00025;
        break;
    case GAIN_SIXTEEN:
        multi = 0.000125;
        break;
    default:
        multi = 0.001;
        break;
    }
    return multi;
}

void
ADS1015::setDelay(){
    switch((int)ADS1X15::getSPS()){
    case 0:
        m_conversionDelay = 8000;
        break;
    case 32:
        m_conversionDelay = 4000;
        break;
    case 64:
        m_conversionDelay = 3000;
        break;
    case 96:
        m_conversionDelay = 1100;
        break;
    case 128:
        m_conversionDelay = 700;
        break;
    case 160:
        m_conversionDelay = 500;
        break;
    case 192:
        m_conversionDelay = 400;
        break;
    default:
        m_conversionDelay = 8000;
        break;
    }
}

ADS1X15::ADSMUXMODE
ADS1015::getMuxMode(unsigned int input) {
    ADS1X15::ADSMUXMODE mode;
    switch (input) {
    case 0:
        return SINGLE_0;
    case 1:
        return SINGLE_1;
    case 2:
        return SINGLE_2;
    case 3:
        return SINGLE_3;
    default:
        UPM_THROW("Invalid input");
    }
}

