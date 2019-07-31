/*
 * Author: Marc Graham <marc@m2ag.net>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#include "ads1115.hpp"
#include "upm_string_parser.hpp"

using namespace upm;

ADS1115::ADS1115(int bus, uint8_t address) : ADS1X15(bus, address) {
    m_name = "ADS1115";
    m_conversionDelay = ADS1115_CONVERSIONDELAY;
    m_bitShift = 0;
    ADS1X15::getCurrentConfig();
}

ADS1115::ADS1115(std::string initStr) : ADS1X15(initStr) {
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
