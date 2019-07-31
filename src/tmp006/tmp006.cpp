/*
 * Author: Norbert Wesp <nwesp@phytec.de>
 * Copyright (c) 2017 Phytec Messtechnik GmbH.
 *
 * based on: RIOT-driver tmp006 by Johann Fischer <j.fischer@phytec.de>
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

#include <unistd.h>
#include <stdlib.h>
#include <endian.h>

#include "tmp006.hpp"

using namespace upm;

TMP006::TMP006(int bus, uint8_t conv_rate, int devAddr) : m_i2ControlCtx(bus) {
    uint8_t reg[2];
    uint16_t tmp;

    m_temperature = 0;

    m_name = TMP006_NAME;

    m_controlAddr = devAddr;

    sensorType = TMP006_SEN;
    m_bus = bus;

    if (conv_rate > TMP006_CONFIG_CR_AS16) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_conv_rate() failed");
    }

    mraa::Result ret = m_i2ControlCtx.address(m_controlAddr);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_address() failed");
    }

    if (checkID() != 0) {
        /* sensor_id does not match! maybe wrong sensor chosen? */
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": checkID() failed");
    }

    tmp = m_i2ControlCtx.readWordReg(TMP006_CONFIGURATION);
    reg[0] = (tmp >> 8);

    tmp = TMP006_CONFIG_CR(conv_rate);
    reg[1] = (tmp >> 8);

    tmp = ((uint16_t)reg[1] << 8) | reg[0];

    ret = m_i2ControlCtx.writeWordReg(TMP006_CONFIGURATION, tmp);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_write_word_data() failed");
    }
}

int
TMP006::checkID(void)
{
    uint8_t tmp[2];
    uint16_t id;
    int re = 0;
    int ret = 0;

    tmp[0] = 0;tmp[1] = 0;
    // Checking if TMP006
    re = m_i2ControlCtx.readBytesReg(TMP006_DEVICE_ID_REG, tmp, 2);
    if (re != 2) {
        /* not enough bytes were read! */
        //return -1;
        ret = -1;
    }

    id = ((uint16_t)tmp[0] << 8) | tmp[1];

    if (id != TMP006_DEVICE_ID) {
        //return -1;
        ret = -1;
    } else {
        sensorType = TMP006_SEN;
        return 0;
    }

    // Checking if TMP007
    re = m_i2ControlCtx.readBytesReg(TMP007_DEVICE_ID_REG, tmp, 2);
    if (re != 2) {
        /* not enough bytes were read! */
        //return -1;
        ret = -1;
    }

    id = ((uint16_t)tmp[0] << 8) | tmp[1];

    if (id != TMP007_DEVICE_ID) {
        //return -1;
        ret = -1;
    } else {
        sensorType = TMP007_SEN;
        return 0;
    }

    return ret;
}

void
TMP006::resetSensor(void)
{
    uint8_t reg[2];
    uint16_t tmp = TMP006_RST_SOFT;
    reg[0] = (tmp >> 8);
    reg[1] = (tmp & 0x00ff);
    tmp = ((uint16_t)reg[1] << 8) | reg[0];

    mraa::Result ret = m_i2ControlCtx.writeWordReg(TMP006_CONFIGURATION, tmp);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_write_word_data() failed");
    }
}

void
TMP006::setActive(void)
{
    uint8_t reg[2];
    uint16_t tmp;

    tmp = m_i2ControlCtx.readWordReg(TMP006_CONFIGURATION);
    reg[0] = (tmp >> 8);
    reg[1] = (tmp & 0x00ff);
    reg[1] |= (TMP006_CONFIG_MOD(TMP006_CONFIG_MOD_CC) >> 8);

    tmp = ((uint16_t)reg[0] << 8) | reg[1];

    mraa::Result ret = m_i2ControlCtx.writeWordReg(TMP006_CONFIGURATION, tmp);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_write_word_data() failed");
    }
}

void
TMP006::setStandby(void)
{
    uint8_t reg[2];
    uint16_t tmp;

    tmp = m_i2ControlCtx.readWordReg(TMP006_CONFIGURATION);
    reg[0] = (tmp >> 8);
    reg[1] = (tmp & 0x00ff);
    reg[1] &= ~(TMP006_CONFIG_MOD(TMP006_CONFIG_MOD_CC) >> 8);

    tmp = ((uint16_t)reg[0] << 8) | reg[1];

    mraa::Result ret = m_i2ControlCtx.writeWordReg(TMP006_CONFIGURATION, tmp);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_write_word_data() failed");
    }
}

int
TMP006::sampleData(void)
{
    uint8_t buf[2];
    uint16_t rawVolt;
    uint16_t rawTemp;
    float tamb[1];
    float tobj[1];
    uint16_t tmp;
    uint8_t drdy[1];

    tmp = m_i2ControlCtx.readWordReg(TMP006_CONFIGURATION);
    buf[0] = (tmp >> 8);
    buf[1] = (tmp & 0x00ff);

    *drdy = buf[0] & (TMP006_DRDY_DATA_RDY);

    if(sensorType == TMP006_SEN) {
        if(! (*drdy)) {
            /* conversation in progress */
            return -1;
        }
    }

    tmp = m_i2ControlCtx.readWordReg(TMP006_SENSOR_VOLTAGE);
    buf[0] = (tmp >> 8);
    buf[1] = (tmp & 0x00ff);
    rawVolt = ((uint16_t)buf[1] << 8) | buf[0];

    tmp = m_i2ControlCtx.readWordReg(TMP006_LOCAL_TEMPERATURE);
    buf[0] = (tmp >> 8);
    buf[1] = (tmp & 0x00ff);
    rawTemp = ((uint16_t)buf[1] << 8) | buf[0];

    convert_data(rawVolt, rawTemp, tamb, tobj);
    m_temperature = (uint16_t)*tobj;

    return 0;
}

void
TMP006::convert_data(int16_t rawv,int16_t rawt, float *tamb, float *tobj)
{
    /* calculate die temperature */
    *tamb = (double)rawt / 128.0;
    /* die temperature in Kelvin */
    double tdie_k = *tamb + 273.15;

    /* calculate sensor voltage */
    double sens_v = (double)rawv * TMP006_CCONST_LSB_SIZE;

    double tdiff = tdie_k - TMP006_CCONST_TREF;
    double tdiff_pow2 = pow(tdiff, 2);

    double s = TMP006_CCONST_S0 * (1 + TMP006_CCONST_A1 * tdiff
                                   + TMP006_CCONST_A2 * tdiff_pow2);

    double v_os = TMP006_CCONST_B0 + TMP006_CCONST_B1 * tdiff
                  + TMP006_CCONST_B2 * tdiff_pow2;

    double f_obj = (sens_v-v_os) + TMP006_CCONST_C2 * pow((sens_v-v_os), 2);

    double t = pow(pow(tdie_k, 4) + (f_obj / s), 0.25);
    /* calculate object temperature in Celsius */
    *tobj = (t - 273.15);
}

float
TMP006::getTemperature(int bSampleData)
{
    if (bSampleData) {
        if( (sampleData()) == -1) {
            /* conversation in progress */
            return (-273.2);
        }
    }
    return (float) m_temperature;
}

uint16_t
TMP006::getConfig(void)
{
    uint8_t buf[2];
    uint16_t tmp;

    tmp = m_i2ControlCtx.readWordReg(TMP006_CONFIGURATION);
    buf[0] = (tmp >> 8);
    buf[1] = (tmp & 0x00ff);

    return ((uint16_t)buf[1] << 8) | buf[0];
}
