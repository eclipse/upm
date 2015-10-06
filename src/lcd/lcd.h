/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
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
#include <mraa.h>
#include <mraa/types.hpp>

namespace upm
{
/**
 * @brief I2C LCD Display library
 * @defgroup i2clcd libupm-i2clcd
 * @ingroup seeed sparkfun adafruit i2c gpio display gsk
 */
class LCD
{
  public:
    LCD();
    virtual ~LCD();
    mraa::Result write(int x, int y, std::string msg);

    virtual mraa::Result write(std::string msg) = 0;
    virtual mraa::Result setCursor(int row, int column) = 0;
    virtual mraa::Result clear() = 0;
    virtual mraa::Result home() = 0;

    std::string name();

  protected:
    std::string m_name;
};
}
