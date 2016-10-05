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



/*=========================================================================
    CONVERSION DELAY (in microS)
    -----------------------------------------------------------------------*/
    #define ADS1115_CONVERSIONDELAY         (8000)
/*=========================================================================*/


/*=========================================================================
    CONFIG REGISTER

  -----------------------------------------------------------------------*/

#define ADS1115_DR_MASK      (0x00E0)
#define ADS1115_DR_8SPS      (0x0000)  // 8 samples per second
#define ADS1115_DR_16SPS     (0x0020)  // 16 samples per second
#define ADS1115_DR_32SPS     (0x0040)  // 32 samples per second
#define ADS1115_DR_64SPS     (0x0060)  // 64 samples per second
#define ADS1115_DR_128SPS    (0x0080)  // 128 samples per second (default)
#define ADS1115_DR_250SPS    (0x00A0)  // 250 samples per second
#define ADS1115_DR_475SPS    (0x00C0)  // 475 samples per second
#define ADS1115_DR_860SPS    (0x00E0)  // 860 samples per second

/*=========================================================================*/

namespace upm {
  /**
   * @library ads1x15
   * @sensor ADS1115
   * @comname ADS1115 ADC
   * @type electric
   * @man ti adafruit
   * @con i2c
   * @web http://www.ti.com/lit/ds/symlink/ads1115.pdf
   *
   * @brief API for ADS1115
   *
   * The ADS1113, ADS1114, and ADS1115 are precision analog-to-digital converters (ADCs) with 16 bits of resolution offered
   * in an ultra-small, leadless QFN-10 package or an MSOP-10 package. The ADS1113/4/5 are designed with precision, power,
   * and ease of implementation in mind. The ADS1113/4/5 feature an onboard reference and oscillator. Data is transferred via
   * an I2C-compatible serial interface; four I2C slave addresses can be selected. The ADS1113/4/5 operate from a single power
   * supply ranging from 2.0V to 5.5V.
   * The ADS1113/4/5 can perform conversions at rates up to 860 samples per second (SPS). An onboard PGA is available on
   * the ADS1114 and ADS1115 that offers input ranges from the supply to as low as +/- 256mV, allowing both large and small
   * signals to be measured with high resolution. The ADS1115 also features an input multiplexer (MUX) that provides two
   * differential or four single-ended inputs.
   * The ADS1113/4/5 operate either in continuous conversion mode or a single-shot mode that automatically powers down after
   * a conversion and greatly reduces current consumption during idle periods. The ADS1113/4/5 are specified from -40 deg C to +125 deg C.
   *
   * Tested with DIYMall ADS1115 board. Also available from Adafruit: https://www.adafruit.com/products/1085
   *
   * @image html ads1115.jpg
   * @snippet ads1x15.cxx Interesting
   */
    class ADS1115 : public ADS1X15 {

        public:

            /**
             * @enum ADSSAMPLERATE
             * @brief uint16_t enum containing values
             * representing the sample rate of the device.
             *
             * @var ADSSAMPLERATE::SPS_8   = 0x0000
             * @var ADSSAMPLERATE::SPS_16  = 0x0020
             * @var ADSSAMPLERATE::SPS_32  = 0x0040
             * @var ADSSAMPLERATE::SPS_64  = 0x0060
             * @var ADSSAMPLERATE::SPS_128 = 0x0080
             * @var ADSSAMPLERATE::SPS_250 = 0x00A0
             * @var ADSSAMPLERATE::SPS_475 = 0x00C0
             * @var ADSSAMPLERATE::SPS_860 = 0x00E0
             */
            typedef enum ADSDATARATE
            {
              SPS_8            = ADS1115_DR_8SPS,
              SPS_16           = ADS1115_DR_16SPS,
              SPS_32           = ADS1115_DR_32SPS,
              SPS_64           = ADS1115_DR_64SPS,
              SPS_128          = ADS1115_DR_128SPS,
              SPS_250          = ADS1115_DR_250SPS,
              SPS_475          = ADS1115_DR_475SPS,
              SPS_860          = ADS1115_DR_860SPS
            } ADSDATARATE;



            /**
             * ADS1X15 constructor
             *
             * @param bus i2c bus the sensor is attached to.
             * @param address. Device address. Default is 0x48.
             */
            ADS1115 (int bus, uint8_t address = 0x48);

            /**
             * ADS1X15 destructor
             */
            ~ADS1115();

            /**
             * Sets the sample rate of the device. This function
             * needs to be overridden in subclasses as the ADS1115 and
             * ADS1015 have different sample rates.
             *
             * @param ADSSAMPLERATE enum
             */
            void setSPS(ADSDATARATE rate = ADS1115::SPS_128);


        protected:
          float getMultiplier(void);
          void setDelay(void);

    };
}
