/*
 * Author: Noel Eck <noel.eck@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdexcept>
#include <string>

#include "ims.hpp"

using namespace upm;

IMS::IMS(int16_t i2c_bus, int16_t i2c_address) :
    _dev(ims_init(i2c_bus, i2c_address))
{
    if (_dev == NULL)
        throw std::runtime_error(std::string(__FUNCTION__) +
                ": failed to initialize sensor, check syslog");
}

uint16_t IMS::get_version()
{
    uint16_t retval;
    if (ims_get_version(_dev, &retval) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                ": ims_get_version command failed");
    return retval;
}

uint16_t IMS::get_moisture()
{
    uint16_t retval;
    if (ims_get_moisture(_dev, &retval) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                ": ims_get_moisture command failed");
    return retval;
}

int IMS::getMoisture()
{
  return get_moisture();
}

uint16_t IMS::get_light()
{
    uint16_t retval;
    if (ims_get_light(_dev, &retval) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                ": ims_get_light command failed");
    return retval;
}

float IMS::get_temperature()
{
    uint16_t retval;
    if (ims_get_temperature(_dev, &retval) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                ": ims_get_temperature command failed");
    return static_cast<float>(retval)/10.0;
}

float IMS::getTemperature()
{
    return get_temperature();
}

void IMS::reset_i2c_address(uint8_t address_new)
{
    if (ims_reset_i2c_address(_dev, address_new) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                ": reset_i2c_address command failed");
}

void IMS::reset()
{
    if ( ims_write(_dev, IMS_RESET, 1) != UPM_SUCCESS )
        throw std::runtime_error(std::string(__FUNCTION__) +
                ": ims_write command failed");
}

uint16_t IMS::read(IMS_RD_COMMAND cmd)
{
    uint16_t retval;
    if (ims_read(_dev, cmd, &retval) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                ": ims_read command failed");
    return retval;
}

void IMS::write(IMS_WR_COMMAND cmd, uint8_t value)
{
    if ( ims_write(_dev, cmd, value) != UPM_SUCCESS )
        throw std::runtime_error(std::string(__FUNCTION__) +
                ": ims_write command failed");
}

void IMS::sleep()
{
    if ( ims_sleep(_dev) != UPM_SUCCESS )
        throw std::runtime_error(std::string(__FUNCTION__) +
                ": ims_sleep command failed");
}
