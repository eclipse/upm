/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * Based on UPM C++ drivers originally developed by:
 * Author: Daniel Mosquera
 * Copyright (c) 2013 Daniel Mosquera
 *
 * Author: Thomas Ingleby <thomas.c.ingleby@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Contributions: Sergey Kiselev <sergey.kiselev@intel.com>
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <string>
#include <stdexcept>
#include <unistd.h>

#include "lcm1602.hpp"

using namespace upm;

Lcm1602::Lcm1602(int bus_in, int addr_in, bool isExpander,
                 uint8_t numColumns, uint8_t numRows) :
    m_lcm1602(lcm1602_i2c_init(bus_in, addr_in, isExpander, numColumns,
                               numRows))
{
    if (!m_lcm1602)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": lcm1602_i2c_init failed");
}

Lcm1602::Lcm1602(int rs,  int enable, int d0,
                 int d1, int d2, int d3,
                 uint8_t numColumns, uint8_t numRows) :
    m_lcm1602(lcm1602_gpio_init(rs, enable, d0, d1, d2, d3, numColumns,
                                numRows))
{
    if (!m_lcm1602)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": lcm1602_gpio_init failed");
}

Lcm1602::~Lcm1602()
{
    lcm1602_close(m_lcm1602);
}

upm_result_t Lcm1602::write(std::string msg)
{
    return lcm1602_write(m_lcm1602, (char *)msg.data(), msg.size());
}

upm_result_t Lcm1602::setCursor(int row, int column)
{
    return lcm1602_set_cursor(m_lcm1602, row, column);
}

upm_result_t Lcm1602::clear()
{
    return lcm1602_clear(m_lcm1602);
}

upm_result_t Lcm1602::home()
{
    return lcm1602_home(m_lcm1602);
}

upm_result_t Lcm1602::createChar(uint8_t charSlot,
                                 std::vector<uint8_t> charData)
{
    return lcm1602_create_char(m_lcm1602, charSlot, (char *)charData.data());
}

upm_result_t Lcm1602::displayOn()
{
    return lcm1602_display_on(m_lcm1602, true);
}

upm_result_t Lcm1602::displayOff()
{
    return lcm1602_display_on(m_lcm1602, false);
}

upm_result_t Lcm1602::cursorOn()
{
    return lcm1602_cursor_on(m_lcm1602, true);
}

upm_result_t Lcm1602::cursorOff()
{
    return lcm1602_cursor_on(m_lcm1602, false);
}

upm_result_t Lcm1602::cursorBlinkOn()
{
    return lcm1602_cursor_blink_on(m_lcm1602, true);
}

upm_result_t Lcm1602::cursorBlinkOff()
{
    return lcm1602_cursor_blink_on(m_lcm1602, false);
}

upm_result_t Lcm1602::backlightOn()
{
    return lcm1602_backlight_on(m_lcm1602, true);
}

upm_result_t Lcm1602::backlightOff()
{
    return lcm1602_backlight_on(m_lcm1602, false);
}

upm_result_t Lcm1602::scrollDisplayLeft()
{
    return lcm1602_scroll_display_left(m_lcm1602);
}

upm_result_t Lcm1602::scrollDisplayRight()
{
    return lcm1602_scroll_display_right(m_lcm1602);
}

upm_result_t Lcm1602::entryLeftToRight()
{
    return lcm1602_entry_left_to_right(m_lcm1602, true);
}

upm_result_t Lcm1602::entryRightToLeft()
{
    return lcm1602_entry_left_to_right(m_lcm1602, false);
}

upm_result_t Lcm1602::autoscrollOn()
{
    return lcm1602_autoscroll_on(m_lcm1602, true);
}

upm_result_t Lcm1602::autoscrollOff()
{
    return lcm1602_autoscroll_on(m_lcm1602, false);
}

upm_result_t Lcm1602::command(uint8_t cmd)
{
    return lcm1602_command(m_lcm1602, cmd);
}

upm_result_t Lcm1602::data(uint8_t data)
{
    return lcm1602_data(m_lcm1602, data);
}
