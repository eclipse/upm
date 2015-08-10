/*
 * Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
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

#include <unistd.h>
#include <stdint.h>
#include <string>
#include <iostream>

#include <mraa/gpio.h>


// TM1637-specific register addresses for writing all digits at a time
#define TM1637_ADDR    0x40
#define TM1637_REG     0xC0
#define TM1637_CMD     0x88

// Display-specific values
#define M_DISPLAY_DIGITS  4

namespace upm
{

/**
 * @brief TM1637 7-Segment Display library
 * @defgroup tm1637 libupm-tm1637
 * @ingroup seeed gpio display
 */

/**
 * @library tm1637
 * @sensor tm1637
 * @comname TM1637 7-Segment Display
 * @altname Grove 4-Digit Display
 * @type display
 * @man seeed
 * @con gpio
 *
 * @brief API for the TM1637 7-Segment Display
 *
 * TM1637 is a display controller for LED-based 7-segment displays.
 * It can be used to address and write data to multiple display digits. This
 * driver is based on the Grove version of the TM1637 display that uses 4
 * digits, thus making it ideal for clock displays, timers, counters, or even
 * score displays in a two-player arcade game.
 *
 * @image html tm1637.jpeg
 * @snippet tm1637.cxx Interesting
 */

  class TM1637
  {
  public:
      /**
       * Enum for the memory-mapped GPIO
       */
      typedef enum {
        NO  = 0,
        YES = 1
      } M_FAST_GPIO;
      /**
       * TM1637 constructor
       *
       * @param clk_pin Clock pin the sensor is connected to
       * @param dio_pin Data pin the sensor is connected to
       * @param bright Initial brightness, from 0 (dark) to 7 (bright) (default is 3)
       * @param mmio Fast memory-mapped GPIO writes; default is yes
       */
      TM1637(int clk_pin, int dio_pin, int bright = 3, M_FAST_GPIO mmio = YES);
      /**
       * TM1637 destructor
       */
      ~TM1637();
      /**
       * Writes digits to the display in a 7-segment encoding
       *
       * @param digits Array of digits to send to the display
       * @return 0 if successful, error code otherwise
       */
      mraa_result_t write(uint8_t *digits);
      /**
       * Writes digits to the display in a 7-segment encoding
       *
       * @param d List of multiple arguments to send to the display
       * @return 0 if successful, error code otherwise
       */
      mraa_result_t write(int d, ...);
      /**
       * Writes a symbol (digit or letter) to the display in a specified index
       *
       * @param index 0-based index of the digit to change from the left
       * @param symbol Digit or letter to display
       * @return 0 if successful, error code otherwise
       */
      mraa_result_t writeAt(int index, char symbol);
      /**
       * Writes all the digits or letters to the display as a string
       *
       * @param digits String of symbols to display
       * @return 0 if successful, error code otherwise
       */
      mraa_result_t write(std::string digits);
      /**
       * Toggles the colon between digits on the display
       *
       * @param value True to turn the colon on, false to turn it off
       */
      void setColon(bool value);
      /**
       * Controls the brightness of the display
       *
       * @param value Brightness, from 0 (darkest) to 7 (brightest)
       */
      void setBrightness(int value);

  private:
      void i2c_start();
      void i2c_stop();
      void i2c_writeByte(uint8_t value);
      void update();
      uint8_t encode(char c);

      mraa_gpio_context m_clk, m_dio;
      std::string m_name;
      uint8_t m_digits[4];
      uint8_t m_brightness;
  };
}
