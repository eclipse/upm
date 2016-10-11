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

#include <string>
#include <stdexcept>
#include <unistd.h>

#include "jhd1313m1.hpp"

using namespace upm;

Jhd1313m1::Jhd1313m1(int bus, int lcdAddress, int rgbAddress) :
    m_jhd1313m1(jhd1313m1_init(bus, lcdAddress, rgbAddress))
{
    if (!m_jhd1313m1)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": jhd1313m1_init failed");
}

Jhd1313m1::~Jhd1313m1()
{
    jhd1313m1_close(m_jhd1313m1);
}

upm_result_t Jhd1313m1::write(std::string msg)
{
    return jhd1313m1_write(m_jhd1313m1, (char *)msg.data(), msg.size());
}

upm_result_t Jhd1313m1::setCursor(int row, int column)
{
    return jhd1313m1_set_cursor(m_jhd1313m1, row, column);
}

upm_result_t Jhd1313m1::scroll(bool direction)
{
    return jhd1313m1_scroll(m_jhd1313m1, direction);
}

upm_result_t Jhd1313m1::setColor(uint8_t r, uint8_t g, uint8_t b)
{
    return jhd1313m1_set_color(m_jhd1313m1, r, g, b);
}

upm_result_t Jhd1313m1::clear()
{
    return jhd1313m1_clear(m_jhd1313m1);
}

upm_result_t Jhd1313m1::home()
{
    return jhd1313m1_home(m_jhd1313m1);
}

upm_result_t Jhd1313m1::createChar(uint8_t charSlot,
                                 jhd1313m1_custom_char_t charData)
{
    return jhd1313m1_create_char(m_jhd1313m1, charSlot, charData);
}

upm_result_t Jhd1313m1::displayOn()
{
    return jhd1313m1_display_on(m_jhd1313m1, true);
}

upm_result_t Jhd1313m1::displayOff()
{
    return jhd1313m1_display_on(m_jhd1313m1, false);
}

upm_result_t Jhd1313m1::cursorOn()
{
    return jhd1313m1_cursor_on(m_jhd1313m1, true);
}

upm_result_t Jhd1313m1::cursorOff()
{
    return jhd1313m1_cursor_on(m_jhd1313m1, false);
}

upm_result_t Jhd1313m1::cursorBlinkOn()
{
    return jhd1313m1_cursor_blink_on(m_jhd1313m1, true);
}

upm_result_t Jhd1313m1::cursorBlinkOff()
{
    return jhd1313m1_cursor_blink_on(m_jhd1313m1, false);
}

upm_result_t Jhd1313m1::backlightOn()
{
    return jhd1313m1_backlight_on(m_jhd1313m1, true);
}

upm_result_t Jhd1313m1::backlightOff()
{
    return jhd1313m1_backlight_on(m_jhd1313m1, false);
}

upm_result_t Jhd1313m1::scrollDisplayLeft()
{
    return jhd1313m1_scroll_display_left(m_jhd1313m1);
}

upm_result_t Jhd1313m1::scrollDisplayRight()
{
    return jhd1313m1_scroll_display_right(m_jhd1313m1);
}

upm_result_t Jhd1313m1::entryLeftToRight()
{
    return jhd1313m1_entry_left_to_right(m_jhd1313m1, true);
}

upm_result_t Jhd1313m1::entryRightToLeft()
{
    return jhd1313m1_entry_left_to_right(m_jhd1313m1, false);
}

upm_result_t Jhd1313m1::autoscrollOn()
{
    return jhd1313m1_autoscroll_on(m_jhd1313m1, true);
}

upm_result_t Jhd1313m1::autoscrollOff()
{
    return jhd1313m1_autoscroll_on(m_jhd1313m1, false);
}

upm_result_t Jhd1313m1::command(uint8_t cmd)
{
    return jhd1313m1_command(m_jhd1313m1, cmd);
}

upm_result_t Jhd1313m1::data(uint8_t data)
{
    return jhd1313m1_data(m_jhd1313m1, data);
}
