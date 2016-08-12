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
#pragma once

#include <string>
#include <ostream>
#include <cstdint>
#include <map>
#include <vector>

#include "types/upm_analog.h"
#include "mraa/aio.h"

namespace upm
{
/**
 * @brief Abstract base class for analog sensors
 * @defgroup analog libupm-ainBase
 * @ingroup analog
 */
class ainBase
{
public:
    /*
     * @brief Constructor for abstract analog sensor class
     * @param pin Target analog pin
     * @param aref ADC reference voltage used for all pins in this sensor
     * @param scale Scale sensor output value
     * @param offset Offset value in sensor units
     */
    ainBase(int16_t pin, float aref = 5.0, float scale = 1.0, float offset = 0.0);
    ainBase();

    /*
     * @brief Destructor for abstract analog sensor class
     */
    virtual ~ainBase();

    /*
     * @brief Pure virtual method for sensor name
     */
    virtual std::string name() const = 0;

    /*
     * @brief Pure virtual method for sensor description
     */
    virtual std::string description() const = 0;

    /*
     * @brief Pure virtual method for sensor ouput units
     */
    virtual std::string units() const = 0;

    /*
     * @brief String representation of this analog sensor
     */
    virtual std::string toString() const;

    /*
     * @brief Add additional analog pins to this sensor
     * @param pin Target analog pin
     * @param scale Scale sensor output value
     * @param offset Offset value in sensor units
     */
    virtual void addPin(int16_t pin, float aref = 5.0, float scale = 1.0, float offset = 0.0);
    virtual void addPin(int16_t pin, analog_sensor_t* aio);

    /*
     * @brief Set ADC reference voltage
     * @param aref Target ADC reference voltage
     */
    virtual void setAref(float aref = 5.0, int16_t pin = -1);

    /*
     * @brief Get ADC reference voltage
     * @return  ADC reference voltage
     */
    virtual float getAref(int16_t pin = -1) const;

    /*
     * @brief Set sensor offset
     * @param offset Offset value in sensor units
     * @param pin Target analog pin.  A value of -1 auto-selects the pin
     */
    virtual void setOffset(float offset = 0.0, int16_t pin = -1);

    /*
     * @brief Get sensor offset
     * @param pin Target analog pin.  A value of -1 auto-selects the pin
     * @return Offset value in sensor units
     */
    virtual float getOffset(int16_t pin = -1) const;

    /*
     * @brief Set sensor scale
     * @param scale Scale sensor output value
     * @param pin Target analog pin.  A value of -1 auto-selects the pin
     */
    virtual void setScale(float scale = 1.0, int16_t pin = -1);

    /*
     * @brief Set sensor scale
     * @param pin Target analog pin.  A value of -1 auto-selects the pin
     * @return Scale value
     */
    virtual float getScale(int16_t pin = -1) const;

    /*
     * @brief Get raw counts from ADC
     * @param pin Target analog pin.  A value of -1 auto-selects the pin
     * @return Raw ADC value in counts
     */
    virtual int32_t getRawCounts(int16_t pin = -1) const;

    /*
     * @brief Get normalized ADC value 0.0 -> 1.0
     * @param pin Target analog pin.  A value of -1 auto-selects the pin
     * @return Normalized ADC value
     */
    virtual float getRawNormalized(int16_t pin = -1) const;

    /*
     * @brief Get raw ADC voltage in volts (v)
     * @param pin Target analog pin.  A value of -1 auto-selects the pin
     * @return Raw ADC value in volts (v)
     */
    virtual float getRawVolts(int16_t pin = -1) const;

    /*
     * @brief Get the sensor adjusted output in native units
     *      This is a pure virtual method, meant to be provided by the derived
     *      class.  There are many ways to get the output sensor value, in
     *      general it could look something like:
     *
     *          units = ADCnormalized * Max Sensor out * scale + offset
     *
     * @param pin Target analog pin.  A value of -1 auto-selects the pin
     * @return Scaled, offset, native units
     */
    virtual float getSensorValue(int16_t pin = -1) const = 0;

    virtual std::vector<int16_t> getPins() const;

protected:
    /* Map of analog pin to mraa aio context */
    std::map<int16_t, analog_sensor_t*> _pinmap;
};

/* Overload the ostream operator for printing analog sensors */
std::ostream& operator<<(std::ostream &os, const ainBase& ab);
}
