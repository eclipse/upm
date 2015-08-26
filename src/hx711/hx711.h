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
      * @brief HX711 24-bit ADC library
      * @defgroup hx711 libupm-hx711
      * @ingroup generic gpio electric
      */

     /**
      * @library hx711
      * @sensor hx711
      * @comname HX711 Analog-to-Digital Converter
      * @type electric
      * @man generic
      * @web http://www.dfrobot.com/image/data/SEN0160/hx711_english.pdf
      * @con gpio
      *
      * @brief API for the HX711 Analog-to-Digital Converter
      *
      * HX711 is a precision 24-bit analog-to-digital converter (ADC)
      * designed for weight scales and industrial control applications to
      * interface directly with a bridge sensor. This module was tested on
      * the Intel(R) Galileo Gen 2 board.
      *
      * @image html hx711.jpeg
      * @snippet hx711.cxx Interesting
      */
      class HX711 {
      public:
            /**
            * HX711 constructor
            *
            * @param data Defines the data pin
            * @param sck Defines the clock pin
            * @param gain Defines the gain factor
            * Valid values are 128 or 64 for channel A; channel B works with a 32-gain factor only
            */
            HX711(uint8_t data, uint8_t sck, uint8_t gain = 128);

            /**
            * HX711 destructor
            */
            ~HX711();

            /**
            * Waits for the chip to be ready and returns a reading
            *
            * @return Raw ADC reading
            */
            unsigned long read();

            /**
            * Sets the gain factor; takes effect only after a call to read()
            * channel A can be set for a 128 or 64 gain; channel B has a fixed 32-gain
            * factor depending on the parameter; the channel is also set to either A or B
            * @param gain Defines the gain factor
            */
            void setGain(uint8_t gain = 128);

            /**
            * Returns an average reading
            * @param times Defines how many reading to do
            * @return Average reading
            */
            unsigned long readAverage(uint8_t times = 10);

            /**
            * Returns (readAverage() - OFFSET)
            * @param times Defines how many readings to do
            * @return Current value without the tare weight
            */
            double getValue(uint8_t times = 10);

            /**
            * Returns getValue() divided by SCALE
            * @param times Defines how many readings to do
            * @return Raw value divided by a value obtained via calibration
            */
            float getUnits(uint8_t times = 1);

            /**
            * Sets the OFFSET value for the tare weight
            * @param times Defines how many times to read the tare value
            */
            void tare(uint8_t times = 10);

            /**
            * Sets the SCALE value
            * This value is used to convert the raw data to human-readable data (measurement units)
            * @param scale Value obtained via calibration
            */
            void setScale(float scale = 1.f);
       private:
            mraa_gpio_context m_sckPinCtx; // Power Down and Serial Clock Input Pin
            mraa_gpio_context m_dataPinCtx; // Serial Data Output Pin

            uint8_t GAIN; // amplification factor
            unsigned long OFFSET; // used for tare weight
            float SCALE; // used to return weight in grams, kg, ounces, whatever


            /**
            * Sets the OFFSET value
            * This value is subtracted from the actual reading (tare weight)
            * @param scale Value obtained via calibration
            */
            void setOffset(long offset = 0);
     };

}
