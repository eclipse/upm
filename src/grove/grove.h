/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Copyright (c) 2014 Intel Corporation.
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
#include <maa/aio.h>
#include <maa/gpio.h>

namespace upm {

class Grove {
    public:
        virtual ~Grove() {}
        std::string name()
        {
            return m_name;
        }
    protected:
        std::string m_name;
};

class GroveLed: public Grove {
    public:
        GroveLed(int pin);
        ~GroveLed();
        maa_result_t write(int value);
        maa_result_t off();
        maa_result_t on();
    private:
        maa_gpio_context* m_gpio;
};

class GroveTemp: public Grove {
    public:
        GroveTemp(unsigned int pin);
        ~GroveTemp();
        float raw_value();
        int value();
    private:
        maa_aio_context* m_aio;
};

class GroveLight: public Grove {
    public:
        GroveLight(unsigned int pin);
        ~GroveLight();
        float raw_value();
        int value();
    private:
        maa_aio_context* m_aio;
};

}
