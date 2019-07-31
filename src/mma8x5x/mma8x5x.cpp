/*
 * Author: Norbert Wesp <nwesp@phytec.de>
 * Copyright (c) 2017 Phytec Messtechnik GmbH.
 *
 * based on: RIOT-driver mma8x5x by Johann Fischer <j.fischer@phytec.de>
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
#include <time.h>

#include "mma8x5x.hpp"

using namespace upm;

MMA8X5X::MMA8X5X (int bus, mma8x5x_params_t *params,
                                         int devAddr) : m_i2ControlCtx(bus) {
    uint8_t reg;

    s_data->x = 0;
    s_data->y = 0;
    s_data->z = 0;

    m_controlAddr = devAddr;
    m_bus = bus;

    mraa::Result ret = m_i2ControlCtx.address(m_controlAddr);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_address() failed");
    }

    if (setDeviceParams(params)) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": setDeviceParams() failed");
    }

    /* reset device */
    ret = m_i2ControlCtx.writeReg(MMA8X5X_CTRL_REG2, MMA8X5X_CTRL_REG2_RST);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_write_byte_data() failed");
    }
    do {
        usleep(500000);
        reg = m_i2ControlCtx.readReg(MMA8X5X_CTRL_REG2);
    } while (reg & MMA8X5X_CTRL_REG2_RST);

    /* configure the user offset */
    ret = m_i2ControlCtx.writeReg(MMA8X5X_OFF_X, s_params->offsetX);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_write_byte_data() failed");
    }
    ret = m_i2ControlCtx.writeReg(MMA8X5X_OFF_Y, s_params->offsetY);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_write_byte_data() failed");
    }
    ret = m_i2ControlCtx.writeReg(MMA8X5X_OFF_Z, s_params->offsetZ);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_write_byte_data() failed");
    }

    /* configure range */
    reg = (s_params->range & MMA8X5X_XYZ_DATA_CFG_FS_MASK);
    ret = m_i2ControlCtx.writeReg(MMA8X5X_XYZ_DATA_CFG, reg);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_write_byte_data() failed");
    }

    /* configure rate and activate the device */
    reg = ((s_params->rate & MMA8X5X_CTRL_REG1_DR_MASK) |
           MMA8X5X_CTRL_REG1_ACTIVE);
    ret = m_i2ControlCtx.writeReg(MMA8X5X_CTRL_REG1, reg);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_write_byte_data() failed");
    }
}

int
MMA8X5X::setDeviceName(uint8_t type)
{
    uint8_t id;

    if (type == 0){
        id = m_i2ControlCtx.readReg(MMA8X5X_DEVICE_ID_REG);
    } else {
        id = type;
    }

    switch (id) {
        case (MMA8X5X_DEVICE_ID_MMA8652):
            m_name = MMA8X5X_NAME_MMA8652;
            s_params->type = MMA8X5X_DEVICE_ID_MMA8652;
            break;
        case (MMA8X5X_DEVICE_ID_MMA8653):
            m_name = MMA8X5X_NAME_MMA8653;
            s_params->type = MMA8X5X_DEVICE_ID_MMA8653;
            break;
        case (MMA8X5X_DEVICE_ID_MMA8451):
            m_name = MMA8X5X_NAME_MMA8451;
            s_params->type = MMA8X5X_DEVICE_ID_MMA8451;
            break;
        case (MMA8X5X_DEVICE_ID_MMA8452):
            m_name = MMA8X5X_NAME_MMA8452;
            s_params->type = MMA8X5X_DEVICE_ID_MMA8452;
            break;
        case (MMA8X5X_DEVICE_ID_MMA8453):
            m_name = MMA8X5X_NAME_MMA8453;
            s_params->type = MMA8X5X_DEVICE_ID_MMA8453;
            break;
        default:
            return -1;
    }

    return 0;
}

int
MMA8X5X::setDeviceParams (mma8x5x_params_t* params)
{
    if (params == NULL) {
        if (setDeviceName(0)) {
            return -1;
        }

        s_params->rate = MMA8X5X_RATE_DEFAULT;
        s_params->range = MMA8X5X_RANGE_DEFAULT;
        s_params->offsetX = 0;
        s_params->offsetY = 0;
        s_params->offsetZ = 0;
    } else {
        if (params->type != MMA8X5X_DEVICE_ID_MMA8652 &&
            params->type != MMA8X5X_DEVICE_ID_MMA8653 &&
            params->type != MMA8X5X_DEVICE_ID_MMA8451 &&
            params->type != MMA8X5X_DEVICE_ID_MMA8452 &&
            params->type != MMA8X5X_DEVICE_ID_MMA8453) {
            /* given type is not supported */
            return -1;
        }
        setDeviceName(params->type);

        if (params->rate != MMA8X5X_RATE_800HZ &&
            params->rate != MMA8X5X_RATE_400HZ &&
            params->rate != MMA8X5X_RATE_200HZ &&
            params->rate != MMA8X5X_RATE_100HZ &&
            params->rate != MMA8X5X_RATE_50HZ &&
            params->rate != MMA8X5X_RATE_1HZ25 &&
            params->rate != MMA8X5X_RATE_6HZ25 &&
            params->rate != MMA8X5X_RATE_1HZ56) {
            /* given rate is not supported */
            return -1;
        }
        s_params->rate = params->rate;

        if (params->range != MMA8X5X_RANGE_2G &&
            params->range != MMA8X5X_RANGE_4G &&
            params->range != MMA8X5X_RANGE_8G) {
            /* given range is not supported */
            return -1;
        }
        s_params->range = params->range;

        s_params->offsetX = params->offsetX;
        s_params->offsetY = params->offsetY;
        s_params->offsetZ = params->offsetZ;
    }

    return 0;
}

int
MMA8X5X::setUserOffset(int8_t x, int8_t y, int8_t z)
{
    mraa::Result ret = m_i2ControlCtx.writeReg(MMA8X5X_OFF_X, (uint8_t)x);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_write_byte_data() failed");
        return -1;
    }
    ret = m_i2ControlCtx.writeReg(MMA8X5X_OFF_Y, (uint8_t)y);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_write_byte_data() failed");
        return -1;
    }
    ret = m_i2ControlCtx.writeReg(MMA8X5X_OFF_Z, (uint8_t)z);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_write_byte_data() failed");
        return -1;
    }

    return 0;
}

int
MMA8X5X::setActive(void)
{
    uint8_t reg;

    reg = m_i2ControlCtx.readReg(MMA8X5X_CTRL_REG1);
    reg |= MMA8X5X_CTRL_REG1_ACTIVE;

    mraa::Result ret = m_i2ControlCtx.writeReg(MMA8X5X_CTRL_REG1, reg);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_write_byte_data() failed");
        return -1;
    }
    return 0;
}

int
MMA8X5X::setStandby(void)
{
    uint8_t reg;

    reg = m_i2ControlCtx.readReg(MMA8X5X_CTRL_REG1);
    reg &= ~MMA8X5X_CTRL_REG1_ACTIVE;

    mraa::Result ret = m_i2ControlCtx.writeReg(MMA8X5X_CTRL_REG1, reg);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_write_byte_data() failed");
        return -1;
    }
    return 0;
}

int
MMA8X5X::isReady(void)
{
    uint8_t reg;

    reg = m_i2ControlCtx.readReg(MMA8X5X_STATUS);

    if (reg & MMA8X5X_STATUS_ZYXDR) {
        return MMA8X5X_DATA_READY;
    } else {
        return MMA8X5X_NODATA;
    }
}

int
MMA8X5X::sampleData(void)
{
    uint8_t buf[7];
    int re = 0;

    re = m_i2ControlCtx.readBytesReg(MMA8X5X_STATUS, buf, 7);
    if (re != 7) {
        /* did not read enough bytes */
        return -1;
    }

    s_data->x = ((int16_t)(buf[1] << 8 | buf[2])) / (16 >> s_params->range);
    s_data->y = ((int16_t)(buf[3] << 8 | buf[4])) / (16 >> s_params->range);
    s_data->z = ((int16_t)(buf[5] << 8 | buf[6])) / (16 >> s_params->range);

    return 0;
}

int16_t
MMA8X5X::getX(int bSampleData)
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
MMA8X5X::getY(int bSampleData)
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
MMA8X5X::getZ(int bSampleData)
{
    if (bSampleData) {
        if (sampleData() != 0) {
            /* error in read values from reg */
            return -999;
        }
    }

    return s_data->z;
}

std::vector<float>
MMA8X5X::getAcceleration()
{
    sampleData();

    std::vector<float> v(3);
    v[0] = s_data->x;
    v[1] = s_data->y;
    v[2] = s_data->z;

    return v;
}

int
MMA8X5X::getData(mma8x5x_data_t* data, int bSampleData)
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

    return 0;
}