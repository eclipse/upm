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

#include "max30100.hpp"

using namespace upm;

void max30100_throw(std::string func, std::string cmd, upm_result_t result)
{
    throw std::runtime_error(func + ": " + cmd + " failed, " +
            "upm_result_t: " + std::to_string(result));
}

MAX30100::MAX30100(int16_t i2c_bus) : _dev(max30100_init(i2c_bus))
{
    if (_dev == NULL)
        throw std::runtime_error(std::string(__FUNCTION__) +
                ": failed to initialize sensor, check syslog");
}

void _read_sample_proxy(max30100_value sample, void* _max30100)
{
    if ((_max30100 != NULL) && ((MAX30100*)_max30100)->_callback != NULL)
        ((MAX30100*)_max30100)->_callback->run(sample);
}

max30100_value MAX30100::sample()
{
    max30100_value retval;
    upm_result_t result = max30100_sample(_dev, &retval);
    if (result != UPM_SUCCESS)
        max30100_throw(__FUNCTION__, "max30100_sample", result);
    return retval;
}


void MAX30100::sample_continuous(int gpio_pin, bool buffered, Callback *cb)
{
    // Use a default callback if one is NOT provided
    if (cb == NULL)
        _callback = (Callback *)&_default_callback;
    else
        _callback = cb;
    max30100_sample_continuous(_dev, gpio_pin, buffered, &_read_sample_proxy, this);
}

void MAX30100::sample_stop()
{
    upm_result_t result = max30100_sample_stop(_dev);
    if (result != UPM_SUCCESS)
        max30100_throw(__FUNCTION__, "max30100_sample_stop" , result);
}

uint16_t MAX30100::version()
{
    uint16_t retval;
    upm_result_t result = max30100_get_version(_dev, &retval);
    if (result != UPM_SUCCESS)
        max30100_throw(__FUNCTION__, "max30100_get_version" , result);
    return retval;
}

float MAX30100::temperature()
{
    float retval;
    upm_result_t result = max30100_get_temperature(_dev, &retval);
    if (result != UPM_SUCCESS)
        max30100_throw(__FUNCTION__, "max30100_get_temperature", result);
    return retval;
}

void MAX30100::mode(MAX30100_MODE mode)
{
    upm_result_t result = max30100_set_mode(_dev, mode);
    if (result != UPM_SUCCESS)
        max30100_throw(__FUNCTION__, "max30100_set_mode", result);
}

MAX30100_MODE MAX30100::mode()
{
    MAX30100_MODE mode;
    upm_result_t result =  max30100_get_mode(_dev, &mode);
    if (result != UPM_SUCCESS)
        max30100_throw(__FUNCTION__, "max30100_get_mode", result);
    return mode;
}

void MAX30100::high_res_enable(bool enable)
{
    upm_result_t result = max30100_set_high_res(_dev, enable);
    if (result != UPM_SUCCESS)
        max30100_throw(__FUNCTION__, "max30100_set_high_res", result);
}

bool MAX30100::high_res_enable()
{
    bool enabled;
    upm_result_t result =  max30100_get_high_res(_dev, &enabled);
    if (result != UPM_SUCCESS)
        max30100_throw(__FUNCTION__, "max30100_get_high_res", result);
    return enabled;
}

void MAX30100::sample_rate(MAX30100_SR sample_rate)
{
    upm_result_t result = max30100_set_sample_rate(_dev, sample_rate);
    if (result != UPM_SUCCESS)
        max30100_throw(__FUNCTION__, "max30100_set_sample_rate", result);
}

MAX30100_SR MAX30100::sample_rate()
{
    MAX30100_SR sample_rate;
    upm_result_t result =  max30100_get_sample_rate(_dev, &sample_rate);
    if (result != UPM_SUCCESS)
        max30100_throw(__FUNCTION__, "max30100_get_sample_rate", result);
    return sample_rate;
}

void MAX30100::pulse_width(MAX30100_LED_PW pulse_width)
{
    upm_result_t result = max30100_set_pulse_width(_dev, pulse_width);
    if (result != UPM_SUCCESS)
        max30100_throw(__FUNCTION__, "max30100_set_pulse_width", result);
}

MAX30100_LED_PW MAX30100::pulse_width()
{
    MAX30100_LED_PW pulse_width;
    upm_result_t result =  max30100_get_pulse_width(_dev, &pulse_width);
    if (result != UPM_SUCCESS)
        max30100_throw(__FUNCTION__, "max30100_get_pulse_width", result);
    return pulse_width;
}

void MAX30100::current(MAX30100_LED_CURRENT ir, MAX30100_LED_CURRENT r)
{
    upm_result_t result =  max30100_set_current(_dev, ir, r);
    if (result != UPM_SUCCESS)
        max30100_throw(__FUNCTION__, "set_current", result);
}

MAX30100_LED_CURRENT MAX30100::current_ir()
{
    MAX30100_LED_CURRENT ir, r;
    upm_result_t result =  max30100_get_current(_dev, &ir, &r);
    if (result != UPM_SUCCESS)
        max30100_throw(__FUNCTION__, "get_current_ir", result);
    return ir;
}

MAX30100_LED_CURRENT MAX30100::current_r()
{
    MAX30100_LED_CURRENT ir, r;
    upm_result_t result =  max30100_get_current(_dev, &ir, &r);
    if (result != UPM_SUCCESS)
        max30100_throw(__FUNCTION__, "get_current_r", result);
    return r;
}

void MAX30100::reset()
{
    upm_result_t result =  max30100_reset(_dev);
    if (result != UPM_SUCCESS)
        max30100_throw(__FUNCTION__, "max30100_reset", result);
}

uint8_t MAX30100::read(MAX30100_REG reg)
{
    uint8_t retval;
    upm_result_t result = max30100_read(_dev, reg, &retval);
    if (result != UPM_SUCCESS)
        max30100_throw(__FUNCTION__, "max30100_read", result);
    return retval;
}

void MAX30100::write(MAX30100_REG reg, uint8_t value)
{
    upm_result_t result =  max30100_write(_dev, reg, value);
    if (result != UPM_SUCCESS)
        max30100_throw(__FUNCTION__, "max30100_write", result);
}

void MAX30100::sleep(bool sleep)
{
    upm_result_t result =  max30100_sleep(_dev, sleep);
    if (result != UPM_SUCCESS)
        max30100_throw(__FUNCTION__, "max30100_sleep", result);
}
