/*
 * Author: Noel Eck <noel.eck@intel.com>
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

#include <iostream>
#include <iomanip>
#include <sstream>

#include "ainBase.hpp"

using namespace upm;

std::ostream& upm::operator<<(std::ostream &os, const ainBase& ab)
{
    os << std::fixed << std::setprecision(3);
    os << "Sensor: " << ab.name() << std::endl
       << "Description: " << ab.description() << std::endl
       << "ADC ref(v): " << ab.getAref() << std::endl
       << "Units: " << ab.units();
    std::vector<int16_t> pins = ab.getPins();
    for (std::vector<int16_t>::const_iterator it = pins.begin();
            it != pins.end(); ++it)
        os <<std::endl << "  Pin [" << *it << "] Scale: " << ab.getScale(*it)
           << " Offset: " << ab.getOffset(*it)
           << " Value(" << ab.units() << "): " << ab.getSensorValue(*it)
           << " Volts(v): " << ab.getRawVolts(*it);
    return os;
}

ainBase::ainBase() {}

ainBase::ainBase(int16_t pin, float aref, float scale, float offset)
{
    addPin(pin, aref, scale, offset);
}

ainBase::~ainBase()
{
    /* Delete all mraa contexts */
    for (std::map<int16_t, analog_sensor_t*>::const_iterator it =
            _pinmap.begin(); it != _pinmap.end(); ++it)
        mraa_aio_close(it->second->aio);
}

std::string ainBase::toString() const
{
    std::ostringstream os;
    os << static_cast<const ainBase&>(*this);
    return os.str();
}

void ainBase::addPin(int16_t pin, float aref, float scale, float offset)
{
    analog_sensor_t* ast;

    /* Don't allow a double context */
    if (_pinmap.find(pin) != _pinmap.end())
        ast = _pinmap[pin];
    else
        ast = new analog_sensor_t;

    ast->aio = mraa_aio_init(pin);

    /* Throw if mraa C call fails */
    if (!ast->aio) throw std::invalid_argument(std::string(__FUNCTION__) +
            ": mraa_aio_init() failed, invalid pin?");

    ast->aref = aref;
    ast->scale = scale;
    ast->offset = offset;

    addPin(pin, ast);
}

void ainBase::addPin(int16_t pin, analog_sensor_t* aio)
{
    // Add pin to context map
    _pinmap[pin] = aio;
}

void ainBase::setAref(float aref, int16_t pin)
{
    /* Use default or throw if invalid pin */
    if (pin == -1)
        pin = _pinmap.begin()->first;
    else if (_pinmap.find(pin) == _pinmap.end())
        throw std::invalid_argument(std::string(__FUNCTION__) +
                ": Invalid AIO pin");

    _pinmap[pin]->aref = aref;
}

float ainBase::getAref(int16_t pin) const
{
    std::map<int16_t, analog_sensor_t*>::const_iterator it =
        _pinmap.begin();

    /* Use default or throw if invalid pin */
    if (pin != -1)
        it = _pinmap.find(pin);

    if (it == _pinmap.end())
        throw std::invalid_argument(std::string(__FUNCTION__) +
                ": Invalid AIO pin");

    return it->second->aref;
}

void ainBase::setOffset(float offset, int16_t pin)
{
    /* Use default or throw if invalid pin */
    if (pin == -1)
        pin = _pinmap.begin()->first;
    else if (_pinmap.find(pin) == _pinmap.end())
        throw std::invalid_argument(std::string(__FUNCTION__) +
                ": Invalid AIO pin");

    _pinmap[pin]->offset = offset;
}

float ainBase::getOffset(int16_t pin) const
{
    std::map<int16_t, analog_sensor_t*>::const_iterator it =
        _pinmap.begin();

    /* Use default or throw if invalid pin */
    if (pin != -1)
        it = _pinmap.find(pin);

    if (it == _pinmap.end())
        throw std::invalid_argument(std::string(__FUNCTION__) +
                ": Invalid AIO pin");

    return it->second->offset;
}

void ainBase::setScale(float scale, int16_t pin)
{
    /* Use default or throw if invalid pin */
    if (pin == -1)
        pin = _pinmap.begin()->first;
    else if (_pinmap.find(pin) == _pinmap.end())
        throw std::invalid_argument(std::string(__FUNCTION__) +
                ": Invalid AIO pin");

    _pinmap[pin]->scale = scale;
}

float ainBase::getScale(int16_t pin) const
{
    std::map<int16_t, analog_sensor_t*>::const_iterator it =
        _pinmap.begin();

    /* Use default or throw if invalid pin */
    if (pin != -1)
        it = _pinmap.find(pin);

    if (it == _pinmap.end())
        throw std::invalid_argument(std::string(__FUNCTION__) +
                ": Invalid AIO pin");

    return it->second->scale;
}

int32_t ainBase::getRawCounts(int16_t pin) const
{
    std::map<int16_t, analog_sensor_t*>::const_iterator it =
        _pinmap.begin();

    /* Use default or throw if invalid pin */
    if (pin != -1)
        it = _pinmap.find(pin);

    if (it == _pinmap.end())
        throw std::invalid_argument(std::string(__FUNCTION__) +
                ": Invalid AIO pin");

    return mraa_aio_read(it->second->aio);
}

float ainBase::getRawNormalized(int16_t pin) const
{
    std::map<int16_t, analog_sensor_t*>::const_iterator it =
        _pinmap.begin();

    /* Use default or throw if invalid pin */
    if (pin != -1)
        it = _pinmap.find(pin);

    if (it == _pinmap.end())
        throw std::invalid_argument(std::string(__FUNCTION__) +
                ": Invalid AIO pin");

    return mraa_aio_read_float(it->second->aio);
}

float ainBase::getRawVolts(int16_t pin) const
{
    float norm = getRawNormalized(pin);
    if (norm == -1.0) return norm;

    return norm * getAref(pin);
}

std::vector<int16_t> ainBase::getPins() const
{
    std::vector<int16_t> pins;
    for (std::map<int16_t, analog_sensor_t*>::const_iterator it =
            _pinmap.begin(); it != _pinmap.end(); ++it)
        pins.push_back(it->first);
    return pins;
}
