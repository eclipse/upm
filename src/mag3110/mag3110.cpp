/*
 * Author: Norbert Wesp <nwesp@phytec.de>
 * Copyright (c) 2017 Phytec Messtechnik GmbH.
 *
 * based on: RIOT-driver mag3110 by Johann Fischer <j.fischer@phytec.de>
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

#include "mag3110.hpp"

using namespace upm;

MAG3110::MAG3110 (int bus, uint8_t dros, int devAddr) : m_i2ControlCtx(bus) {
    s_data->x = 0;
    s_data->y = 0;
    s_data->z = 0;
    s_data->status = 0;
    s_data->dtemp = 0;

    uint8_t reg;

    m_name = MAG3110_NAME;

    m_controlAddr = devAddr;
    m_bus = bus;

    if (dros > MAG3110_DROS_0008_128) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": given DR-OS invalid");
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

    /* enable automatic magnetic sensor reset */
    reg = MAG3110_CTRL_REG2_AUTO_MRST_EN;
    ret = m_i2ControlCtx.writeReg(MAG3110_CTRL_REG2, reg);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_write_byte_data() failed");
    }

    reg = MAG3110_CTRL_REG1_DROS(dros);
    ret = m_i2ControlCtx.writeReg(MAG3110_CTRL_REG1, reg);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_write_byte_data() failed");
    }
}

int
MAG3110::checkID(void)
{
    uint8_t id;

    id = m_i2ControlCtx.readReg(MAG3110_DEVICE_ID_REG);

    if (id != MAG3110_DEVICE_ID) {
        return -1;
    }

    return 0;
}

int
MAG3110::setUserOffset(int16_t x, int16_t y, int16_t z)
{
    mraa::Result ret;
    ret = m_i2ControlCtx.writeWordReg(MAG3110_OFF_X_MSB, (uint16_t)x);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_write_word_data() failed");
        return -1;
    }

    ret = m_i2ControlCtx.writeWordReg(MAG3110_OFF_Y_MSB, (uint16_t)y);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_write_word_data() failed");
        return -1;
    }

    ret = m_i2ControlCtx.writeWordReg(MAG3110_OFF_Z_MSB, (uint16_t)z);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_write_word_data() failed");
        return -1;
    }

    return 0;
}

int
MAG3110::setActive(void)
{
    uint8_t reg;

    reg = m_i2ControlCtx.readReg(MAG3110_CTRL_REG1);

    reg |= MAG3110_CTRL_REG1_AC;

    mraa::Result ret = m_i2ControlCtx.writeReg(MAG3110_CTRL_REG1, reg);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_write_byte_data() failed");
        return -1;
    }

    return 0;
}

int
MAG3110::setStandby(void)
{
    uint8_t reg;

    reg = m_i2ControlCtx.readReg(MAG3110_CTRL_REG1);

    reg &= ~MAG3110_CTRL_REG1_AC;

    mraa::Result ret = m_i2ControlCtx.writeReg(MAG3110_CTRL_REG1, reg);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_write_byte_data() failed");
        return -1;
    }

    return 0;
}

int
MAG3110::isReady(void)
{
    uint8_t reg;

    reg = m_i2ControlCtx.readReg(MAG3110_DR_STATUS);

    return (int)(reg & MAG3110_DR_STATUS_ZYXDR);
}

int
MAG3110::sampleData(void)
{
    uint8_t buf[7];
    uint8_t dtemp;
    int re = 0;

    re = m_i2ControlCtx.readBytesReg(MAG3110_DR_STATUS, buf, 7);
    if (re != 7) {
        /* did not read enough bytes */
        return -1;
    }

    s_data->status = buf[0];
    s_data->x = ((int16_t)buf[1] << 8) | buf[2];
    s_data->y = ((int16_t)buf[3] << 8) | buf[4];
    s_data->z = ((int16_t)buf[5] << 8) | buf[6];

    dtemp = m_i2ControlCtx.readReg(MAG3110_DIE_TEMP);

    s_data->dtemp = dtemp;

    return 0;
}

std::vector<float> MAG3110::getMagnetometer()
{
    uint8_t buf[7];
    int re = 0;

    re = m_i2ControlCtx.readBytesReg(MAG3110_DR_STATUS, buf, 7);
    if (re != 7) {
        /* did not read enough bytes */
        return {-1, -1, -1};
    }

    s_data->status = buf[0];
    s_data->x = ((int16_t)buf[1] << 8) | buf[2];
    s_data->y = ((int16_t)buf[3] << 8) | buf[4];
    s_data->z = ((int16_t)buf[5] << 8) | buf[6];

    s_data->dtemp = m_i2ControlCtx.readReg(MAG3110_DIE_TEMP);

    return {(float)s_data->x, (float)s_data->y, (float)s_data->z};

}

int16_t
MAG3110::getX(int bSampleData)
{
    if (bSampleData) {
        if (sampleData() != 0) {
            /* error in read values from reg */
            return -999;
        }
    }

    return s_data->x;
}

int16_t
MAG3110::getY(int bSampleData)
{
    if (bSampleData) {
        if (sampleData() != 0) {
            /* error in read values from reg */
            return -999;
        }
    }

    return s_data->y;
}

int16_t
MAG3110::getZ(int bSampleData)
{
    if (bSampleData) {
        if (sampleData() != 0) {
            /* error in read values from reg */
            return -999;
        }
    }

    return s_data->z;
}

uint8_t
MAG3110::getStatus(void)
{
    return s_data->status;
}

int8_t
MAG3110::getDieTemperature(void)
{
    return s_data->dtemp;
}

int
MAG3110::getData(mag3110_data_t* data, int bSampleData)
{
    if (bSampleData) {
        if (sampleData() != 0) {
            /* error in read values from reg */
            return -1;
        }
    }

    data->x = s_data->x;
    data->y = s_data->y;
    data->z = s_data->z;
    data->status = s_data->status;
    data->dtemp = s_data->dtemp;

    return 0;
}
