/*
 * Author: Marc Graham <marc@m2ag.net>
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
/*=========================================================================*/

#pragma once

#include "ads1x15.hpp"
#include "upm/iADC.hpp"

#define ADS1015_VREF 2.048

/*=========================================================================
    CONVERSION DELAY (in microS)
    -----------------------------------------------------------------------*/
    #define ADS1015_CONVERSIONDELAY         (8000)
/*=========================================================================*/


/*=========================================================================
    CONFIG REGISTER

  -----------------------------------------------------------------------*/

#define ADS1015_DR_MASK      (0x00E0)
#define ADS1015_DR_128SPS    (0x0000)  // 128 samples per second
#define ADS1015_DR_250SPS    (0x0020)  // 250 samples per second
#define ADS1015_DR_490SPS    (0x0040)  // 490 samples per second
#define ADS1015_DR_920SPS    (0x0060)  // 920 samples per second
#define ADS1015_DR_1600SPS   (0x0080)  // 1600 samples per second (default)
#define ADS1015_DR_2400SPS   (0x00A0)  // 2400 samples per second
#define ADS1015_DR_3300SPS   (0x00C0)  // 3300 samples per second

/*=========================================================================*/

namespace upm {
  /**
   * @library ads1x15
   * @sensor ADS1015
   * @comname ADS1015 ADC
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
   * on the ADS1014 and ADS1015 that offers input ranges from the supply to as low as +/- 256mV, allowing both large and small
   * signals to be measured with high resolution. The ADS1015 also features an input multiplexer (MUX) that provides two
   * differential or four single-ended inputs.
   * The ADS1013/4/5 operate either in continuous conversion mode or a single-shot mode that automatically powers down
   * after a conversion and greatly reduces current consumption during idle periods. The ADS1013/4/5 are specified from
   * -40 deg C to +125 deg C.
   *
   * Tested with Adafriut ADS1015 board: https://www.adafruit.com/products/1083
   *
   * @image html ads1015.jpg
   * @snippet ads1x15.cxx Interesting
   */
    class ADS1015 : public ADS1X15, public IADC {

        public:

        /**
         * @enum ADSSAMPLERATE
         * @brief uint16_t enum containing values
         * representing the sample rate of the device.
         *
         * @var ADSSAMPLERATE::SPS_128  = 0x0000
         * @var ADSSAMPLERATE::SPS_250  = 0x0020
         * @var ADSSAMPLERATE::SPS_490  = 0x0040
         * @var ADSSAMPLERATE::SPS_920  = 0x0060
         * @var ADSSAMPLERATE::SPS_1600 = 0x0080
         * @var ADSSAMPLERATE::SPS_2400 = 0x00A0
         * @var ADSSAMPLERATE::SPS_3300 = 0x00C0
         */
        typedef enum ADSSAMPLERATE {
         SPS_128           = ADS1015_DR_128SPS,
         SPS_250           = ADS1015_DR_250SPS,
         SPS_490           = ADS1015_DR_490SPS,
         SPS_920           = ADS1015_DR_920SPS,
         SPS_1600          = ADS1015_DR_1600SPS,
         SPS_2400          = ADS1015_DR_2400SPS,
         SPS_3300          = ADS1015_DR_3300SPS
        } ADSSAMPLERATE;


            /**
             * ADS1015 constructor
             *
             * This constructor includes a vref parameter that can be used
             * to set gain so it matches full scale value of input
             *
             * @param bus i2c bus the sensor is attached to.
             * @param address. Optional device address. Default is 0x48.
             * @param vref. Optional reference (i.e. half full swing) voltage. Default is 2.048V
             */
            ADS1015 (int bus, uint8_t address = 0x48, float vref = ADS1015_VREF);

            /**
             * ADS1X15 destructor
             */
            ~ADS1015 ();

            /**
             * Sets the sample rate of the device. This function
             * needs to be overridden in subclasses as the ADS1115 and
             * ADS1015 have different sample rates.
             *
             * @param ADSSAMPLERATE enum
             */
            void setSPS(ADSSAMPLERATE rate = SPS_1600);

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

            /**
             * Read current voltage for current single ended analogue input
             *
             * @return current voltage
             */
            unsigned int getResolutionInBits();

            /**
             * Returns module name
             *
             * @return modulename as const char*
             */
            const char* getModuleName();


        protected:
            float getMultiplier(void);
            void setDelay(void);

        private:
            ADS1X15::ADSMUXMODE getMuxMode(unsigned int input);

    };
}
