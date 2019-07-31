/*
 * Authors: Brendan Le Foll <brendan.le.foll@intel.com>
 *          Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 *          Sarah Knepper <sarah.knepper@intel.com>
 *          Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 - 2017 Intel Corporation.
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

#include "light.hpp"

using namespace upm;

Light::Light(unsigned int pin) :
    m_light(light_init(pin))
{
    if ( !m_light )
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": light_init() failed.");
}

Light::~Light()
{
    light_close(m_light);
}

int Light::value()
{
    float value;

    if (light_get_lux(m_light, &value))
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": light_get_normalized() failed.");

    return (int)roundf(value);
}

float Light::getLuminance()
{
    float value;

    if (light_get_lux(m_light, &value))
        throw std::runtime_error(std::string(__FUNCTION__) +
        ": light_get_normalized() failed.");

    return value;
}

float Light::raw_value()
{
    // This is a hack.  Deprecated.  Should be removed ASAP.
   int value =  mraa_aio_read(m_light->aio);
   if (value < 0)
       throw std::runtime_error(std::string(__FUNCTION__) +
                                ": mraa_aio_read() failed.");

   return (float)value; // ?? why a float?
}

void Light::setAref(float aref)
{
    light_set_aref(m_light, aref);
}

void Light::setScale(float scale)
{
    light_set_scale(m_light, scale);
}

void Light::setOffset(float offset)
{
    light_set_aref(m_light, offset);
}

float Light::getAref()
{
    return light_get_aref(m_light);
}

float Light::getScale()
{
    return light_get_scale(m_light);
}

float Light::getOffset()
{
    return light_get_offset(m_light);
}

float Light::getNormalized()
{
    float value;

    if (light_get_normalized(m_light, &value))
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": light_get_normalized() failed.");
    return value;
}

float Light::getRawVolts()
{
    float value;

    if (light_get_raw_volts(m_light, &value))
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": light_get_raw_volts() failed.");
    return value;
}
