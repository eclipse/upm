/*
 * Author: Henry Bruce <henry.bruce@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 * Based on work by Marc Graham <marc@m2ag.net>
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

#include "ads1015_iadc.h"

using namespace upm;


ADS1015_IADC::ADS1015_IADC(int bus, uint8_t address, float vref) : ADS1015(bus, address)
{
   m_vref = vref;
}

ADS1015_IADC::~ADS1015_IADC()
{
}

unsigned int ADS1015_IADC::getNumInputs()
{
    return 4;
}

float ADS1015_IADC::getReferenceVoltage()
{
    return m_vref;
}

unsigned int ADS1015_IADC::getResolutionInBits()
{
    return 12;
}

uint16_t
ADS1015_IADC::getRawValue(unsigned int input)
{
    ADS1X15::ADSMUXMODE mode = getMuxMode(input);
    updateConfigRegister((m_config_reg & ~ADS1X15_MUX_MASK) | mode, true);
    usleep(m_conversionDelay);
    uint16_t value = i2c->readWordReg(ADS1X15_REG_POINTER_CONVERT);
    value = value >> m_bitShift;
    return swapWord(value);
}

float
ADS1015_IADC::getVoltage(unsigned int input)
{
    ADSMUXMODE mode = getMuxMode(input);
    return getSample(mode);
}


ADS1X15::ADSMUXMODE
ADS1015_IADC::getMuxMode(unsigned int input)
{
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
