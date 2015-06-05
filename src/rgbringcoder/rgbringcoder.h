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

#include <iostream>
#include <string>
#include <stdint.h>
#include <unistd.h>
#include <mraa/gpio.hpp>
#include <mraa/pwm.hpp>


namespace upm {
  /**
   * @brief Sparkfun RGB RingCoder
   * @defgroup rgbringcoder libupm-rgbringcoder
   * @ingroup seeed gpio led
   */

  /**
   * @library rgbringcoder
   * @sensor rgbringcoder
   * @comname Sparkfun RGB Ringcoder
   * @type led
   * @web https://www.sparkfun.com/products/11040
   * @man sparkfun
   * @con pwm gpio
   *
   * @brief API for the Sparkfun RGB Ringcoder
   *
   * The RGB RingCoder is a breakout board, a circular LED containing
   * 16 LEDs arranged in a ring, and a rotary encoder.  The encoder
   * contains an RGB LED as well as a push button function.
   *
   * The device will require 11 pins, 3 of which must be PWM capable
   * (for the RGB LEDs).
   *
   * @snippet rgbringcoder.cxx Interesting
   */

  class RGBRingCoder {
  public:

    /**
     * RGBRingCoder constructor
     *
     * @param en enable gpio
     * @param latch latch gpio
     * @param clear clear gpio
     * @param clk clock gpio
     * @param dat data out gpio
     * @param sw push button switch gpio
     * @param encA encoder A gpio
     * @param encB encoder B gpio
     * @param red RGB red led pwm
     * @param green RGB green led pwm
     * @param blue RGB blue led pwm
     */
    RGBRingCoder(int en, int latch, int clear, int clk, int dat, int sw, 
                 int encA, int encB, int red, int green, int blue);

    /**
     * RGBRingCoder Destructor
     */
    ~RGBRingCoder();

    /*
     * set the state of the ring LEDs.  This is a 16 bit value, where
     * each bit corresponds to one of the ring LEDs.  A 1 bit means
     * that specific LED is on, and 0 bit means that specific LED is
     * off.
     *
     * @param bits the bits representing the state of each LED
     */
    void setRingLEDS(uint16_t bits);

    /* 
     * return the state of the button
     *
     * @return true if the button is pressed, false otherwise
     */
    bool getButtonState();

    /* 
     * get the current rotary encoder counter value
     *
     * @return the current counter value
     */
    int getEncoderPosition() { return m_counter; };

    /* 
     * set the encoder counter to 0
     */
    void clearEncoderPosition() { m_counter = 0; };

    /* 
     * set the intensity of the red, grenn, and blue LEDs.  Values can
     * be between 0.0 and 1.0.  Lower is brighter, higher is dimmer.
     *
     * @param r the red value, valid values are between 0.0 and 1.0
     * @param g the green value, valid values are between 0.0 and 1.0
     * @param b the blue value, valid values are between 0.0 and 1.0
     */
    void setRGBLED(float r, float g, float b);

  private:
    
    mraa::Gpio m_gpioEn;

    mraa::Gpio m_gpioLatch;
    mraa::Gpio m_gpioClear;
    mraa::Gpio m_gpioClock;
    mraa::Gpio m_gpioData;

    mraa::Gpio m_gpioSwitch;

    mraa::Pwm m_pwmRed;
    mraa::Pwm m_pwmGreen;
    mraa::Pwm m_pwmBlue;

    mraa::Gpio m_gpioEncA;
    mraa::Gpio m_gpioEncB;

    static void interruptHandler(void *ctx);
    volatile int m_counter;

  };
}


