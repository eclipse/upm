/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>
#include <mraa.h>
#include <mraa/types.hpp>

namespace upm
{
/**
 * @brief I2C LCD Displays Library
 * @defgroup lcd libupm-lcd
 * @ingroup dfrobot sainsmart seeed sparkfun adafruit i2c gpio display gsk
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
