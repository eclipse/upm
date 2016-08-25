/*
 * Author: Jon Trulson <jtrulson@ics.com>
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

#include <string>
#include <stdint.h>
#include <sys/time.h>
#include <mraa/gpio.h>

namespace upm {

/**
 * @brief Grove Rotary Encoder library
 * @defgroup rotaryencoder libupm-rotaryencoder
 * @ingroup seeed gpio other hak
 */
/**
 * @library rotaryencoder
 * @sensor rotaryencoder
 * @comname Grove Rotary Encoder
 * @type other
 * @man seeed
 * @web http://www.seeedstudio.com/wiki/Grove_-_Encoder
 * @con gpio
 * @kit hak
 *
 * @brief API for the Grove Rotary Encoder
 * 
 * UPM module for the Grove rotary encoder. This rotary encoder
 * encodes a rotation signal into electronic pulses that can be used
 * to measure rotation and direction. It is useful in cases where a
 * rotary knob is required, but using a potentiometer is not
 * desirable. A rotary encoder can turn a full 360 degrees
 * without a stop and does not place a resistive load on the
 * circuit, as is the case with a potentiometer.
 *
 * This module maintains a position that is incremented or
 * decremented according to the rotation on the encoder.
 *
 * @image html rotaryencoder.jpg
 * @snippet rotaryencoder.cxx Interesting
 */
  class RotaryEncoder {
  public:
    /**
     * RotaryEncoder constructor
     *
     * @param pinA Digital pin to use for signal A
     * @param pinB Digital pin to use for signal B
     */
    RotaryEncoder(int pinA, int pinB);
    /**
     * RotaryEncoder destructor
     */
    ~RotaryEncoder();

    /**
     * Resets the position to a given number; default is 0. 
     *
     * @param count Integer to initialize the position to
     */
    void initPosition(int count=0);

    /**
     * Gets the position value
     *
     */
    int position();

  private:
    /**
     * Interrupt service routine (ISR) for signal A
     *
     * @param ctx User context for the ISR (*this pointer)
     */
    static void signalAISR(void *ctx);
  
    volatile int m_position;
    mraa_gpio_context m_gpioA;
    mraa_gpio_context m_gpioB;
  };
}


