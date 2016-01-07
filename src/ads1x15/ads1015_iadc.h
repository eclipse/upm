/*
 * Author: Henry Bruce <henry.bruce@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 * Based on work by Marc Graham <marc@m2ag.net>
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
/*=========================================================================*/

#pragma once

#include "ads1015.h"
#include "upm/iADC.h"

#define ADS1015_VREF 3.0

namespace upm {
  /**
   * @library ads1x15
   * @sensor ADS1115
   * @comname ADS1115 ADC
   * @type electric
   * @man ti adafruit
   * @con i2c
   * @web web http://www.ti.com/lit/ds/symlink/ads1015.pdf
   *
   * @brief API for ADS1015
   *
   * The ADS1013, ADS1014, and ADS1015 are precision analog-to-digital converters (ADCs) with 12 bits of resolution
   * offered in an ultra-small, leadless QFN-10 package or an MSOP-10 package. The ADS1013/4/5 are designed with
   * precision, power, and ease of implementation in mind. The ADS1013/4/5 feature an onboard reference and oscillator.
   * Data is transferred via an I2C-compatible serial interface; four I2C slave addresses can be selected. The ADS1013/4/5
   * operate from a single power supply ranging from 2.0V to 5.5V.
   * The ADS1013/4/5 can perform conversions at rates up to 3300 samples per second (SPS). An onboard PGA is available
   * on the ADS1014 and ADS1015 that offers input ranges from the supply to as low as ±256mV, allowing both large and small
   * signals to be measured with high resolution. The ADS1015 also features an input multiplexer (MUX) that provides two
   * differential or four single-ended inputs.
   * The ADS1013/4/5 operate either in continuous conversion mode or a single-shot mode that automatically powers down
   * after a conversion and greatly reduces current consumption during idle periods. The ADS1013/4/5 are specified from
   * –40°C to +125°C.
   *
   * Tested with Adafriut ADS1015 board: https://www.adafruit.com/products/1083
   *
   * @image html ads1015.jpg
   * @snippet ads1x15.cxx Interesting
   */
    class ADS1015_IADC : public ADS1015, public IADC {

        public:

            /**
             * ADS1015_IADC constructor
             *
             * @param bus i2c bus the sensor is attached to.
             * @param address. Device address. Default is 0x48.
             * @param vref reference voltage for this sensor; default is 3.0
             */
            ADS1015_IADC (int bus, uint8_t address = 0x48, float vref = ADS1015_VREF);

            /**
             * ADS1015_IADC destructor
             */
            ~ADS1015_IADC ();

            /**
             * Get number of inputs
             *
             * @return number of inputs
             */
            unsigned int getNumInputs();

            /**
             * Read current value for current single ended analogue input
             *
             * @return current conversion value
             */
            uint16_t getRawValue(unsigned int input);

            /**
             * Read current voltage for current single ended analogue input
             *
             * @return current voltage
             */
            float getVoltage(unsigned int input);

            float getReferenceVoltage();

            unsigned int getResolutionInBits();

            /**
             * Returns whether the sensor is configured.
             */
            bool isConfigured() { return true; }

            const char* getModuleName() { return m_name.c_str(); }

        private:
            ADS1X15::ADSMUXMODE getMuxMode(unsigned int input);
            float m_vref;
    };
}
