/*
 * Author: Jun Kato and Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Contributions: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This module is based on the my9221 driver
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
#include <string>
#include <stdexcept>
#include <unistd.h>
#include <stdlib.h>

#include "grovecircularled.h"

using namespace upm;

GroveCircularLED::GroveCircularLED (uint8_t di, uint8_t dcki) {
  mraa_result_t error = MRAA_SUCCESS;
  
  // init clock context
  m_clkPinCtx = mraa_gpio_init(dcki);
  if (m_clkPinCtx == NULL) {
    throw std::invalid_argument(std::string(__FUNCTION__) +
                                ": mraa_gpio_init(dcki) failed, invalid pin?");
    return;
  }
  mraa_gpio_use_mmaped(m_clkPinCtx, 1);

  // init data context
  m_dataPinCtx = mraa_gpio_init(di);
  if (m_dataPinCtx == NULL) {
    throw std::invalid_argument(std::string(__FUNCTION__) +
                                ": mraa_gpio_init(di) failed, invalid pin?");
    return;
  }

  mraa_gpio_use_mmaped(m_dataPinCtx, 1);

  // set direction (out)
  error = mraa_gpio_dir(m_clkPinCtx, MRAA_GPIO_OUT);
  if (error != MRAA_SUCCESS) {
    mraa_result_print(error);
  }

  // set direction (out)
  error = mraa_gpio_dir(m_dataPinCtx, MRAA_GPIO_OUT);
  if (error != MRAA_SUCCESS) {
    mraa_result_print(error);
  }
}

GroveCircularLED::~GroveCircularLED() {
  mraa_result_t error = MRAA_SUCCESS;
  error = mraa_gpio_close (m_dataPinCtx);
  if (error != MRAA_SUCCESS) {
    mraa_result_print(error);
  }
  error = mraa_gpio_close (m_clkPinCtx);
  if (error != MRAA_SUCCESS) {
    mraa_result_print(error);
  }
}

mraa_result_t
GroveCircularLED::setSpinner (uint8_t position) {
  if (position < 0 || position >= 24) {
    return MRAA_ERROR_INVALID_PARAMETER;
  }
    for(uint8_t block_idx = 0; block_idx < 24; block_idx++) {
      if (block_idx % 12 == 0) {
        send16bitBlock (CMDMODE);
      }
      uint32_t state = (block_idx == position) ? BIT_HIGH : BIT_LOW;
      send16bitBlock (state);
    }
    return lockData ();
}

mraa_result_t
GroveCircularLED::setLevel (uint8_t level, bool direction) {
  if (level < 0 || level > 24) {
    return MRAA_ERROR_INVALID_PARAMETER;
  }
  if (direction) {
    for(uint8_t block_idx = 24; block_idx > 0; block_idx--) {
      if (block_idx % 12 == 0) {
        send16bitBlock (CMDMODE);
      }
      uint32_t state = (block_idx <= level) ? BIT_HIGH : BIT_LOW;
      send16bitBlock (state);
    }
  } else {
    for(uint8_t block_idx = 0; block_idx < 24; block_idx++) {
      if (block_idx % 12 == 0) {
        send16bitBlock (CMDMODE);
      }
      uint32_t state = (block_idx <= level - 1) ? BIT_HIGH : BIT_LOW;
      send16bitBlock (state);
    }
  }
  return lockData ();
}

mraa_result_t
GroveCircularLED::setStatus (bool status[24]) {
  for(uint8_t block_idx = 0; block_idx < 24; block_idx++) {
    if (block_idx % 12 == 0) {
      send16bitBlock (CMDMODE);
    }
    send16bitBlock (status[block_idx] ? BIT_HIGH : BIT_LOW);
  }
  return lockData ();
}

mraa_result_t
GroveCircularLED::lockData () {
  mraa_result_t error = MRAA_SUCCESS;
  error = mraa_gpio_write (m_dataPinCtx, LOW);
  usleep(10);

  for(int idx = 0; idx < 4; idx++) {
    error = mraa_gpio_write (m_dataPinCtx, HIGH);
    error = mraa_gpio_write (m_dataPinCtx, LOW);
  }
  return error;
}

mraa_result_t
GroveCircularLED::send16bitBlock (short data) {
  mraa_result_t error = MRAA_SUCCESS;
  for (uint8_t bit_idx = 0; bit_idx < MAX_BIT_PER_BLOCK; bit_idx++) {
    uint32_t state = (data & 0x8000) ? HIGH : LOW;
    error = mraa_gpio_write (m_dataPinCtx, state);
    state = mraa_gpio_read (m_clkPinCtx);

    if (state) {
      state = LOW;
    } else {
      state = HIGH;
    }

    error = mraa_gpio_write (m_clkPinCtx, state);

    data <<= 1;
  }
  return error;
}
