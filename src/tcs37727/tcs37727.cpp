/*
 * Author: Norbert Wesp <nwesp@phytec.de>
 * Copyright (c) 2017 Phytec Messtechnik GmbH.
 *
 * based on: RIOT-driver tcs37727 by Johann Fischer <j.fischer@phytec.de>
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

#include "tcs37727.hpp"

using namespace upm;

TCS37727::TCS37727(int bus, int atime_us, int devAddr) : m_i2ControlCtx(bus) {
    s_data->red = 0;
    s_data->green = 0;
    s_data->blue = 0;
    s_data->clear = 0;
    s_data->lux = 0;
    s_data->ct = 0;
    s_again = 4;

    m_name = TCS37727_NAME;

    m_controlAddr = devAddr;
    m_bus = bus;

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

    ret = m_i2ControlCtx.writeReg(TCS37727_CONTROL, TCS37727_CONTROL_AGAIN_4);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_write_byte_data() failed");
    }

    ret = m_i2ControlCtx.writeReg(TCS37727_ATIME,
                                             TCS37727_ATIME_TO_REG(atime_us));
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_write_byte_data() failed");
    }

    s_atime_us = atime_us;
}

int
TCS37727::checkID(void)
{
    uint8_t id;

    id = m_i2ControlCtx.readReg(TCS37727_DEVICE_ID_REG);

    if (id != TCS37727_DEVICE_ID) {
        return -1;
    }

    return 0;
}

int
TCS37727::setActive(void)
{
    uint8_t reg;

    reg = m_i2ControlCtx.readReg(TCS37727_ENABLE);

    reg |= (TCS37727_ENABLE_AEN | TCS37727_ENABLE_PON);

    mraa::Result ret = m_i2ControlCtx.writeReg(TCS37727_ENABLE, reg);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_write_byte_data() failed");
        return -1;
    }
    return 0;
}

int
TCS37727::setStandby(void)
{
    uint8_t reg;

    reg = m_i2ControlCtx.readReg(TCS37727_ENABLE);

    reg &= ~TCS37727_ENABLE_AEN;
    if (!(reg & TCS37727_ENABLE_PEN)) {
        reg &= ~TCS37727_ENABLE_PON;
    }

    mraa::Result ret = m_i2ControlCtx.writeReg(TCS37727_ENABLE, reg);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_write_byte_data() failed");
        return -1;
    }
    return 0;
}

uint8_t
TCS37727::trimGain(int rawc)
{
    uint8_t reg_again = 0;
    int val_again = s_again;
    uint8_t reg;

    if (rawc < TCS37727_AG_THRESHOLD_LOW) {
        switch (val_again) {
            case 1:
                reg_again = TCS37727_CONTROL_AGAIN_4;
                val_again = 4;
                break;

            case 4:
                reg_again = TCS37727_CONTROL_AGAIN_16;
                val_again = 16;
                break;

            case 16:
                reg_again = TCS37727_CONTROL_AGAIN_60;
                val_again = 60;
                break;

            case 60:
            default:
                return -1;
        }
    }
    else if (rawc > TCS37727_AG_THRESHOLD_HIGH) {
        switch (val_again) {
            case 60:
                reg_again = TCS37727_CONTROL_AGAIN_16;
                val_again = 16;
                break;

            case 16:
                reg_again = TCS37727_CONTROL_AGAIN_4;
                val_again = 4;
                break;

            case 4:
                reg_again = TCS37727_CONTROL_AGAIN_1;
                val_again = 1;
                break;

            case 1:
            default:
                return -1;
        }
    }
    else {
        return 0;
    }

    reg = m_i2ControlCtx.readReg(TCS37727_CONTROL);

    reg &= ~TCS37727_CONTROL_AGAIN_MASK;
    reg |= reg_again;

    mraa::Result ret = m_i2ControlCtx.writeReg(TCS37727_CONTROL, reg);
    if (ret != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_i2c_write_byte_data() failed");
        return -1;
    }

    s_again = val_again;

    return 0;

}

int
TCS37727::sampleData(void)
{
    uint8_t buf[8];
    int num = 0;

    num = m_i2ControlCtx.readBytesReg((TCS37727_INC_TRANS | TCS37727_CDATA),
                                                                      buf, 8);
    if (num != 8) {
        /* not enough values were read */
        return -1;
    }

    int32_t tmpc = ((uint16_t)buf[1] << 8) | buf[0];
    int32_t tmpr = ((uint16_t)buf[3] << 8) | buf[2];
    int32_t tmpg = ((uint16_t)buf[5] << 8) | buf[4];
    int32_t tmpb = ((uint16_t)buf[7] << 8) | buf[6];

    /* Remove IR component as described in the DN40.  */
    int32_t ir = (tmpr + tmpg + tmpb - tmpc) >> 1;
    tmpr -= ir;
    tmpg -= ir;
    tmpb -= ir;

    /* Color temperature calculation as described in the DN40. */
    int32_t ct = (CT_COEF_IF * tmpb) / tmpr + CT_OFFSET_IF;

    /* Lux calculation as described in the DN40.  */
    int32_t gi = R_COEF_IF * tmpr + G_COEF_IF * tmpg + B_COEF_IF * tmpb;
    /* TODO: add Glass Attenuation Factor GA compensation */
    int32_t cpl = (s_atime_us * s_again) / DGF_IF;
    int32_t lux = gi / cpl;

    /* Autogain */
    trimGain(tmpc);

    s_data->red = (tmpr < 0) ? 0 : (tmpr * 1000) / cpl;
    s_data->green = (tmpg < 0) ? 0 : (tmpg * 1000) / cpl;
    s_data->blue = (tmpb < 0) ? 0 : (tmpb * 1000) / cpl;
    s_data->clear = (tmpb < 0) ? 0 : (tmpc * 1000) / cpl;
    s_data->lux = (lux < 0) ? 0 : lux;
    s_data->ct = (ct < 0) ? 0 : ct;

    return 0;
}

uint32_t
TCS37727::getChannelRed(int bSampleData)
{
    if (bSampleData) {
        if (sampleData() != 0) {
            /* error in read values from reg */
            return 999;
        }
    }

    return s_data->red;
}

uint32_t
TCS37727::getChannelGreen(int bSampleData)
{
    if (bSampleData) {
        if (sampleData() != 0) {
            /* error in read values from reg */
            return 999;
        }
    }

    return s_data->green;
}

uint32_t
TCS37727::getChannelBlue(int bSampleData)
{
    if (bSampleData) {
        if (sampleData() != 0) {
            /* error in read values from reg */
            return 999;
        }
    }

    return s_data->blue;
}

uint32_t
TCS37727::getChannelClear(int bSampleData)
{
    if (bSampleData) {
        if (sampleData() != 0) {
            /* error in read values from reg */
            return 999;
        }
    }

    return s_data->clear;
}

uint32_t
TCS37727::getLux(int bSampleData)
{
    if (bSampleData) {
        if (sampleData() != 0) {
            /* error in read values from reg */
            return 999;
        }
    }

    return s_data->lux;
}

uint32_t
TCS37727::getColorTemperature(int bSampleData)
{
    if (bSampleData) {
        if (sampleData() != 0) {
            /* error in read values from reg */
            return 999;
        }
    }

    return s_data->ct;
}

int
TCS37727::getData(tcs37727_data_t* data, int bSampleData)
{
    if (bSampleData) {
        if (sampleData() != 0) {
            /* error in read values from reg */
            return -1;
        }
    }

    data->red = s_data->red;
    data->green = s_data->green;
    data->blue = s_data->blue;
    data->clear = s_data->clear;
    data->lux = s_data->lux;
    data->ct = s_data->ct;

    return 0;
}
