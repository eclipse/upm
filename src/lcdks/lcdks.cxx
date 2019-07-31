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
 * The MIT License
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

#include "lcdks.hpp"

using namespace upm;

LCDKS::LCDKS(int rs, int enable,
             int d0, int d1, int d2, int d3,
             int keypad, int backlight) :
    m_lcdks(lcdks_init(rs, enable, d0, d1, d2, d3, keypad, backlight))

{
    if (!m_lcdks)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": lcdks_init failed");
}

LCDKS::~LCDKS()
{
    lcdks_close(m_lcdks);
}

upm_result_t LCDKS::write(std::string msg)
{
    return lcdks_write(m_lcdks, (char *)msg.data(), msg.size());
}

upm_result_t LCDKS::setCursor(int row, int column)
{
    return lcdks_set_cursor(m_lcdks, row, column);
}

upm_result_t LCDKS::clear()
{
    return lcdks_clear(m_lcdks);
}

upm_result_t LCDKS::home()
{
    return lcdks_home(m_lcdks);
}

upm_result_t LCDKS::createChar(uint8_t charSlot,
                                   std::vector<uint8_t> charData)
{
    return lcdks_create_char(m_lcdks, charSlot,
                             (char *)charData.data());
}

upm_result_t LCDKS::displayOn()
{
    return lcdks_display_on(m_lcdks, true);
}

upm_result_t LCDKS::displayOff()
{
    return lcdks_display_on(m_lcdks, false);
}

upm_result_t LCDKS::cursorOn()
{
    return lcdks_cursor_on(m_lcdks, true);
}

upm_result_t LCDKS::cursorOff()
{
    return lcdks_cursor_on(m_lcdks, false);
}

upm_result_t LCDKS::cursorBlinkOn()
{
    return lcdks_cursor_blink_on(m_lcdks, true);
}

upm_result_t LCDKS::cursorBlinkOff()
{
    return lcdks_cursor_blink_on(m_lcdks, false);
}

upm_result_t LCDKS::backlightOn()
{
    return lcdks_backlight_on(m_lcdks, true);
}

upm_result_t LCDKS::backlightOff()
{
    return lcdks_backlight_on(m_lcdks, false);
}

upm_result_t LCDKS::scrollDisplayLeft()
{
    return lcdks_scroll_display_left(m_lcdks);
}

upm_result_t LCDKS::scrollDisplayRight()
{
    return lcdks_scroll_display_right(m_lcdks);
}

upm_result_t LCDKS::entryLeftToRight()
{
    return lcdks_entry_left_to_right(m_lcdks, true);
}

upm_result_t LCDKS::entryRightToLeft()
{
    return lcdks_entry_left_to_right(m_lcdks, false);
}

upm_result_t LCDKS::autoscrollOn()
{
    return lcdks_autoscroll_on(m_lcdks, true);
}

upm_result_t LCDKS::autoscrollOff()
{
    return lcdks_autoscroll_on(m_lcdks, false);
}

float LCDKS::getKeyValue()
{
    return lcdks_get_key_value(m_lcdks);
}
