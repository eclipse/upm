/*
 * Author: Rafael da Mata Neri <rafael.neri@gmail.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#pragma once

#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <mraa/gpio.h>

namespace upm {
     /**
      * @brief HX711 24bit ADC library
      * @defgroup hx711 libupm-hx711
      * @ingroup generic gpio electric
      */

     /**
      * @library hx711
      * @sensor hx711
      * @comname HX711 24bit Analog-to-Digital Converter
      * @type electric
      * @man generic
      * @web http://www.dfrobot.com/image/data/SEN0160/hx711_english.pdf
      * @con gpio
      *
      * @brief API for HX711
      *
      * The HX711 is a precision 24-bit analog-to-digital converter (ADC)
      * designed for weigh scales and industrial control applications to
      * interface directly with a bridge sensor. This module was tested on
      * the Intel Galileo Gen2.
      *
      * @image html hx711.jpeg
      * @snippet hx711.cxx Interesting
      */
      class HX711 {
      public:
            /**
            * HX711 module constructor
            *
            * @param data define the data pin
            * @param sck define the clock pin
            * @param gain define the gain factor
            * Valid values are 128 or 64 for channel A; channel B works with 32 gain factor only
            */
            HX711(uint8_t data, uint8_t sck, uint8_t gain = 128);

            /**
            * HX711 module Destructor
            */
            ~HX711();

            /**
            * Waits for the chip to be ready and returns a reading
            *
            * @return raw adc read
            */
            unsigned long read();

            /**
            * Set the gain factor; takes effect only after a call to read()
            * channel A can be set for a 128 or 64 gain; channel B has a fixed 32 gain
            * depending on the parameter, the channel is also set to either A or B
            * @param gain define the gain factor
            */
            void setGain(uint8_t gain = 128);

            /**
            * Returns an average reading
            * @param times define how many times to read
            * @return the avarage reading
            */
            unsigned long readAverage(uint8_t times = 10);

            /**
            * Returns (readAverage() - OFFSET)
            * @param times define how many readings to do
            * @return the current value without the tare weight
            */
            double getValue(uint8_t times = 10);

            /**
            * Returns getValue() divided by SCALE
            * @param times define how many readings to do
            * @return the raw value divided by a value obtained via calibration
            */
            float getUnits(uint8_t times = 1);

            /**
            * Set the OFFSET value for tare weight
            * @param times define how many times to read the tare value
            */
            void tare(uint8_t times = 10);

            /**
            * Set the SCALE value
            * This value is used to convert the raw data to "human readable" data (measure units)
            * @param scale value obtained via calibration
            */
            void setScale(float scale = 1.f);
       private:
            mraa_gpio_context m_sckPinCtx; // Power Down and Serial Clock Input Pin
            mraa_gpio_context m_dataPinCtx; // Serial Data Output Pin

            uint8_t GAIN; // amplification factor
            unsigned long OFFSET; // used for tare weight
            float SCALE; // used to return weight in grams, kg, ounces, whatever


            /**
            * Set the OFFSET value
            * The value that's subtracted from the actual reading (tare weight)
            * @param scale value obtained via calibration
            */
            void setOffset(long offset = 0);
     };

}
