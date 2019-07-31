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

#include <iostream>
#include <unistd.h>

#include "lcd.hpp"

using namespace upm;

LCD::LCD()
{
  m_name = "LCD";
}

LCD::~LCD()
{
}

mraa::Result
LCD::write(int row, int column, std::string msg)
{
    setCursor(row, column);
    return write(msg);
}

std::string
LCD::name()
{
    return m_name;
}
