/*
 * Author: Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <string>
#include <stdexcept>

#include "rsc.hpp"

using namespace upm;
using namespace std;

RSC::RSC(uint8_t bus, uint8_t cs_ee_pin, uint8_t cs_adc_pin) :
    m_rsc(rsc_init(bus, cs_ee_pin, cs_adc_pin))
{
    if(!m_rsc)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                ": rsc_init failed");
}

RSC::~RSC()
{
    rsc_close(m_rsc);
}

string RSC::getSensorName()
{
    uint8_t sensorName[RSC_SENSOR_NAME_LEN]={0};
    if(rsc_get_sensor_name(m_rsc, sensorName) != UPM_SUCCESS) {
        throw std::runtime_error(std::string(__FUNCTION__) +
            ": Unable to read the sensor name");
    }
    std::string str((const char*)sensorName);

    return str;
}

string RSC::getSensorSerialNumber()
{
    uint8_t serialNumber[RSC_SENSOR_NUMBER_LEN]={0};
    if(rsc_get_sensor_serial_number(m_rsc, serialNumber) != UPM_SUCCESS) {
        throw std::runtime_error(std::string(__FUNCTION__) +
            ": Unable to read the sensor serial number");
    }
    std::string str((const char*)serialNumber);

    return str;
}

float RSC::getPressureRange()
{
    return rsc_get_pressure_range(m_rsc);
}

float RSC::getMinimumPressure()
{
    return rsc_get_minimum_pressure(m_rsc);
}

string RSC::getPressureUnit()
{
    string s_unit;
    PRESSURE_U unit = rsc_get_pressure_unit(m_rsc);
    switch(unit){
        case PASCAL:
            s_unit = "Pascal";
        break;
        case KPASCAL:
            s_unit = "Kilo Pascal";
        break;
        case MPASCAL:
            s_unit = "Mega Pascal";
        break;
        case PSI:
            s_unit = "PSI";
        break;
        case INH2O:
            s_unit = "inH2O";
        break;
        case BAR:
            s_unit = "Bar";
        break;
        case mBAR:
            s_unit = "milli Bar";
        break;
        default:
            throw std::runtime_error(std::string(__FUNCTION__) +
                ": Unable to read the pressure unit from sensor");
    }

    return s_unit;
}

string RSC::getPressureType()
{
    string s_type;
    PRESSURE_T type = rsc_get_pressure_type(m_rsc);
    switch(type){
        case DIFFERENTIAL:
            s_type = "Differential";
        break;
        case ABSOLUTE:
            s_type = "Absolute";
        break;
        case GAUGE:
            s_type = "Gauge";
        break;
        default:
            throw std::runtime_error(std::string(__FUNCTION__) +
                ": Unable to read the pressure type from sensor");
    }

    return s_type;
}

uint8_t *RSC::getInitialADCState()
{
    uint8_t initialState[4]={0,0,0,0};
    rsc_get_initial_adc_values(m_rsc, initialState);
    int cnt = 0;
    for (cnt = 0; cnt<4; cnt++)
        m_adc_coeff[cnt]=initialState[cnt];

    return m_adc_coeff;
}

void RSC::updateCoefficients()
{
    if(rsc_retrieve_coefficients(m_rsc) != UPM_SUCCESS) {
        throw std::runtime_error(std::string(__FUNCTION__) +
            ": Unable to retrieve ADC Coefficients from the eeprom");
    }
}

void RSC::setupADC(uint8_t* adc_init_values)
{
    if(rsc_setup_adc(m_rsc, adc_init_values) != UPM_SUCCESS) {
        throw std::runtime_error(std::string(__FUNCTION__) +
            ": Unable to setup the ADC");
    }
}

float RSC::getTemperature()
{
    return rsc_get_temperature(m_rsc);
}

#define INH20_TO_PA 248.84
float RSC::getPressure()
{
    return rsc_get_pressure(m_rsc) * INH20_TO_PA;
}

void RSC::setMode(RSC_MODE mode)
{
    rsc_set_mode(m_rsc, mode);
}

void RSC::setDataRate(RSC_DATA_RATE dr)
{
    rsc_set_data_rate(m_rsc, dr);
}
