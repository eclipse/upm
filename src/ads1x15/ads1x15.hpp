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

#pragma once

#include <iostream>
#include <string>
#include "mraa.hpp"
#include "mraa/i2c.hpp"

/*=========================================================================
    I2C ADDRESS/BITS
    -----------------------------------------------------------------------*/
#define ADS1X15_ADDRESS                 (0x48)    // 1001 000 (ADDR = GND)
/*=========================================================================*/


/*=========================================================================
    POINTER REGISTER
    -----------------------------------------------------------------------*/
#define ADS1X15_REG_POINTER_MASK        (0x03)
#define ADS1X15_REG_POINTER_CONVERT     (0x00)
#define ADS1X15_REG_POINTER_CONFIG      (0x01)
#define ADS1X15_REG_POINTER_LOWTHRESH   (0x02)
#define ADS1X15_REG_POINTER_HITHRESH    (0x03)
/*=========================================================================*/

/*=========================================================================
    CONFIG REGISTER

    -----------------------------------------------------------------------*/
#define ADS1X15_OS_MASK      (0x8000)
#define ADS1X15_OS_SINGLE    (0x8000)  // Write: Set to start a single-conversion
#define ADS1X15_OS_BUSY      (0x0000)  // Read: Bit = 0 when conversion is in progress
#define ADS1X15_OS_NOTBUSY   (0x8000)  // Read: Bit = 1 when device is not performing a conversion

#define ADS1X15_MUX_MASK     (0x7000)
#define ADS1X15_MUX_DIFF_0_1 (0x0000)  // Differential P = AIN0, N = AIN1 (default)
#define ADS1X15_MUX_DIFF_0_3 (0x1000)  // Differential P = AIN0, N = AIN3
#define ADS1X15_MUX_DIFF_1_3 (0x2000)  // Differential P = AIN1, N = AIN3
#define ADS1X15_MUX_DIFF_2_3 (0x3000)  // Differential P = AIN2, N = AIN3
#define ADS1X15_MUX_SINGLE_0 (0x4000)  // Single-ended AIN0
#define ADS1X15_MUX_SINGLE_1 (0x5000)  // Single-ended AIN1
#define ADS1X15_MUX_SINGLE_2 (0x6000)  // Single-ended AIN2
#define ADS1X15_MUX_SINGLE_3 (0x7000)  // Single-ended AIN3

#define ADS1X15_PGA_MASK     (0x0E00)
#define ADS1X15_PGA_6_144V   (0x0000)  // +/-6.144V range = Gain 2/3
#define ADS1X15_PGA_4_096V   (0x0200)  // +/-4.096V range = Gain 1
#define ADS1X15_PGA_2_048V   (0x0400)  // +/-2.048V range = Gain 2 (default)
#define ADS1X15_PGA_1_024V   (0x0600)  // +/-1.024V range = Gain 4
#define ADS1X15_PGA_0_512V   (0x0800)  // +/-0.512V range = Gain 8
#define ADS1X15_PGA_0_256V   (0x0A00)  // +/-0.256V range = Gain 16

#define ADS1X15_MODE_MASK    (0x0100)
#define ADS1X15_MODE_CONTIN  (0x0000)  // Continuous conversion mode
#define ADS1X15_MODE_SINGLE  (0x0100)  // Power-down single-shot mode (default)

#define ADS1X15_DR_MASK      (0x00E0)

#define ADS1X15_CMODE_MASK   (0x0010)
#define ADS1X15_CMODE_TRAD   (0x0000)  // Traditional comparator with hysteresis (default)
#define ADS1X15_CMODE_WINDOW (0x0010)  // Window comparator

#define ADS1X15_CPOL_MASK    (0x0008)
#define ADS1X15_CPOL_ACTVLOW (0x0000)  // ALERT/RDY pin is low when active (default)
#define ADS1X15_CPOL_ACTVHI  (0x0008)  // ALERT/RDY pin is high when active

#define ADS1X15_CLAT_MASK    (0x0400)  // Determines if ALERT/RDY pin latches once asserted
#define ADS1X15_CLAT_NONLAT  (0x0000)  // Non-latching comparator (default)
#define ADS1X15_CLAT_LATCH   (0x0400)  // Latching comparator

#define ADS1X15_CQUE_MASK    (0x0003)
/* This wouldn't compile for the python wrapper. with these in for some reason.
#define ADS1X15_CQUE_1CONV   (0x0000)  // Assert ALERT/RDY after one conversions
#define ADS1X15_CQUE_2CONV     (0x0001)  // Assert ALERT/RDY after two conversions
#define ADS1X15_CQUE_4CONV     (0x0002)  // Assert ALERT/RDY after four conversions
#define ADS1X15_CQUE_NONE      (0x0003)  // Disable the comparator and put ALERT/RDY in high state (default)
*/
/*=========================================================================*/

namespace upm {
  /**
   * @brief ADS1X15 family adc library
   *
   * Library for TI analog to digital converter ic. Base clase fro ADS1X15 provides all the functionality that
   * ADS1115 and ADS1015 ics have in common.
   *
   * @defgroup ads1x15 libupm-ads1x15
   * @ingroup ti adafruit i2c electric
   */
    class ADS1X15 {

        public:

           /**
            * @enum ADSGAIN
            * @brief uint16_t enum containing values for
            * setting gain for ADS1X15 devices.
            *
            * @var ADSGAIN::TWOTHIRDS = 0x0000
            * @var ADSGAIN::ONE       = 0x0200
            * @var ADSGAIN::TWO       = 0x0400
            * @var ADSGAIN::FOUR      = 0x0600
            * @var ADSGAIN::EIGHT     = 0x0800
            * @var ADSGAIN::SIXTEEN   = 0x0A00
            */
            typedef enum ADSGAIN {
              GAIN_TWOTHIRDS    = ADS1X15_PGA_6_144V,
              GAIN_ONE          = ADS1X15_PGA_4_096V,
              GAIN_TWO          = ADS1X15_PGA_2_048V,
              GAIN_FOUR         = ADS1X15_PGA_1_024V,
              GAIN_EIGHT        = ADS1X15_PGA_0_512V,
              GAIN_SIXTEEN      = ADS1X15_PGA_0_256V
            } ADSGAIN ;

            /**
             * @enum ADSMUXMODE
             * @brief uint16_t enum containing values used
             * for selecting ADS1X15 read operations.
             *
             * @var ADSMUXMODE::DIFF_0_1 = 0x0000
             * @var ADSMUXMODE::DIFF_0_2 = 0x1000
             * @var ADSMUXMODE::DIFF_1_3 = 0x2000
             * @var ADSMUXMODE::DIFF_2_3 = 0x3000
             * @var ADSMUXMODE::SINGLE_0 = 0x4000
             * @var ADSMUXMODE::SINGLE_1 = 0x5000
             * @var ADSMUXMODE::SINGLE_2 = 0x6000
             * @var ADSMUXMODE::SINGLE_3 = 0x7000
             */
            typedef enum ADSMUXMODE {
              DIFF_0_1        = ADS1X15_MUX_DIFF_0_1,  // Differential P = AIN0, N = AIN1 (default)
              DIFF_0_3        = ADS1X15_MUX_DIFF_0_3,  // Differential P = AIN0, N = AIN3
              DIFF_1_3        = ADS1X15_MUX_DIFF_1_3,  // Differential P = AIN1, N = AIN3
              DIFF_2_3        = ADS1X15_MUX_DIFF_2_3,  // Differential P = AIN2, N = AIN3
              SINGLE_0        = ADS1X15_MUX_SINGLE_0,  // Single-ended AIN0
              SINGLE_1        = ADS1X15_MUX_SINGLE_1,  // Single-ended AIN1
              SINGLE_2        = ADS1X15_MUX_SINGLE_2,  // Single-ended AIN2
              SINGLE_3        = ADS1X15_MUX_SINGLE_3   // Single-ended AIN3
            } ADSMUXMODE;

            /**
             * @enum ADSCOMP
             * @brief uint16_t enum containing values
             * for setting ADS1X15 comparator queue modes.
             *
             * @var ADSCOMP::CQUE_1CONV = 0x0000
             * @var ADSCOMP::CQUE_2CONV = 0x0001
             * @var ADSCOMP::CQUE_3CONV = 0x0002
             * @var ADSCOMP::CQUE_NONE  = 0x0003
             */
            typedef enum ADSCOMP {
               CQUE_1CONV      = 0x0000,    // Assert ALERT/RDY after one conversions
               CQUE_2CONV      = 0x0001,    // Assert ALERT/RDY after two conversions
               CQUE_4CONV      = 0x0002,    // Assert ALERT/RDY after four conversions
               CQUE_NONE       = 0x0003      // Disable the comparator and put ALERT/RDY in high state (default)
            } ADSCOMP;

            /**
             * @enum ADSTHRESH
             * @brief uint8_t enum containing register addresses
             * used for setting HI and LOW threshold values as
             * well as setting conversion ready and set to default.
             *
             * @var ADSTHRESH::THRESH_LOW     = 0x02
             * @var ADSTHRESH::THRESH_HIGH    = 0x03
             * @var ADSTHRESH::CONVERSION_RDY = 0x04
             * @var ADSTHRESH::THRESH_DEFAULT = 0x05
             */
            typedef enum ADSTHRESH {
                THRESH_LOW      = ADS1X15_REG_POINTER_LOWTHRESH,
                THRESH_HIGH     = ADS1X15_REG_POINTER_HITHRESH,
                CONVERSION_RDY  = 0x04,
                THRESH_DEFAULT  = 0x05

            } ADSTHRESH;

            /**
             * @enum ADSSAMPLERATE
             * @brief uint16_t enum containing values
             * representing the sample rate of the device.
             * Will be overridden in subclass
             *
             * @var ADSSAMPLERATE::SPS_DEFAULT = 0x0080
             */
            typedef enum ADSSAMPLERATE {
               SPS_DEFAULT     = 0x0080
            } ADSSAMPLERATE;

            /**
             * ADS1X15 constructor
             *
             * @param bus i2c bus the sensor is attached to.
             * @param address. Device address. Default is 0x48.
             */
            ADS1X15(int bus, uint8_t address);

            /**
             * ADS1X15 destructor
             */
            virtual ~ADS1X15 ();

            /**
             * Returns the name of the sensor
             */
            std::string name()
            {
                return m_name;
            }

            /**
             *  Returns the contents of conversion register without performing
             *  a conversion operation. Will use a multiplier based on the
             *  current gain setting to give the voltage as a float. Used
             *  internally to return the HI and LOW threshold values.
             *
             *  @param reg uint8_t value specifying register to read.
             *  Should generally be called with no parameter.
             */
            float getLastSample(int reg = ADS1X15_REG_POINTER_CONVERT);

            /**
             * Performs a read as specified by ADS1X15::ADSMUXMOE and
             * returns the value as a float. Uses getLastSample() internally
             * to return voltage value.
             *
             * @pram mode ADSMUXMODE specifying inputs to be sampled.
             */
            float getSample(ADSMUXMODE mode = ADS1X15::DIFF_0_1);

            /**
             * Returns the current gain setting being used by the device
             * as an ADSGAIN value.
             */
            ADSGAIN getGain(){
                return  (ADSGAIN)(m_config_reg & ADS1X15_PGA_MASK);
            }

            /**
             * Sets the PGA gain bits to the desired gain. Default
             * is +/- 2.094 volts.
             *
             * @param gain ADSGAIN value reprenting the desired gain.
             * See warnings in spec sheet.
             */
            void setGain(ADSGAIN gain = ADS1X15::GAIN_TWO);

            /**
             * Returns the current device sample rate a an ADSSAMPLERATE
             * value.
             */
            ADSSAMPLERATE getSPS(void){
                return (ADSSAMPLERATE)(m_config_reg & ADS1X15_DR_MASK);
            }

            /**
             * Sets the sample rate of the device. This function
             * needs to be overrode in subclasses as the ADS1115 and
             * ADS1015 have different data rates.
             *
             * @param ADSSAMPLERATE enum
             * SPS_DEFAULT = 0x0080
             */
            virtual void setSPS(ADSSAMPLERATE rate);

            /**
             * Returns the comparator mode.
             * False = Traditional comparator with Hysteresis (default)
             * True  = Window Comparator
             */
            bool getCompMode(void){
                return (bool)(m_config_reg & ADS1X15_CMODE_MASK);
            }

            /**
             * Sets the comparator mode of the device.
             *
             * @param mode bool value denoting mode.
             * False = Traditional comparator with Hysteresis (default)
             * True  = Window Comparator
             */
            void setCompMode(bool mode = false);

            /**
             * Get comparator polarity. Reports the polarity
             * of the ALERT/RDY pin. Returns:
             * False = Active Low (default)
             * True  = Active High
             */
            bool getCompPol(void){
                return (bool)(m_config_reg & ADS1X15_CPOL_MASK);
            }

            /**
             * Sets the comparator polarity. Controls the
             * polarity of the ALERT/RDY pin.
             *
             * @param mode bool.
             * False = Active Low (default)
             * True  = Active High
             */
            void setCompPol(bool mode = false);

            /**
             * Returns bool representing the state of the
             * comparator latching functionality.
             * False = Non Latching comparator (default)
             * True  = Latching Comparator
             */
            bool getCompLatch(void){
                return (bool)(m_config_reg & ADS1X15_CLAT_MASK);
            }

            /**
             * Sets bit controlling comparator operation.
             *
             * @param mode bool
             * False = Non Latching comparator (default)
             * True  = Latching Comparator
             */
            void setCompLatch(bool mode = false);

            /**
             * Returns ADSCOMP value representing the state of
             * comparator queue.
             *
             * CQUE_1CONV = Assert after one conversion
             * CQUE_2CONV = Assert after two conversions
             * CQUE_2CONV = Assert after four conversions
             * CQUE_NONE = Disable comparator (default)
             */
            ADSCOMP getCompQue(void){
                return (ADSCOMP)(m_config_reg & ADS1X15_CQUE_MASK);
            }

            /**
             * Sets bits controlling Comparator queue operation.
             *
             * @param mode ADSCOMP enum.
             * CQUE_1CONV = Assert after one conversion
             * CQUE_2CONV = Assert after two conversions
             * CQUE_2CONV = Assert after four conversions
             * CQUE_NONE  = Disable comparator (default)
             */
            void setCompQue(ADSCOMP mode = ADS1X15::CQUE_NONE);

            /**
             * Returns bool reflecting state of device mode bit.
             *
             * False = Power Down Single shot mode (default)
             * True  = Continuous conversion mode
             */
            bool getContinuous(void){
                return !(bool)(m_config_reg & ADS1X15_MODE_MASK);
            }

            /**
             * Sets the state of device mode but.
             *
             * @param mode bool
             * False = Power Down Single shot mode (default)
             * True  = Continuous conversion mode
             */
            void setContinuous(bool mode = false);

            /**
             * Returns current high or low threshold setting.
             *
             * @param reg ADSTHRES enum value.
             * Returns 0.0 unless THRESH_HIGH or THRESH_LOW requested.
             */
            float getThresh(ADSTHRESH reg = THRESH_LOW);

            /**
             * Sets threshold levels or configures for conversion ready
             * operation of ALERT/RDY output.
             *
             * @param reg ADSTHRESH enum
             * @param value float value to set threshold register to.
             *
             * THRESH_LOW      = Sets low thresh register.
             * THRESH_HIGH     = Sets high thresh register.
             * CONVERSION_RDY  = Configures conversion ready operation
             * THRESH_DEFAULT  = resets high/low registers to startup values.
             */
            void setThresh(ADSTHRESH reg = THRESH_DEFAULT , float value = 0.0);

        protected:
            std::string m_name;
            float m_conversionDelay;
            uint8_t m_bitShift;
            uint16_t m_config_reg;
            //Must be overridden in subclass for proper values.
            virtual float getMultiplier(void) = 0;
            //Must be overridden in subclass for proper values.
            virtual void setDelay(void) = 0;
            void getCurrentConfig();
            void updateConfigRegister(uint16_t update, bool read = false);
            uint16_t swapWord(uint16_t value);

               mraa::I2c* i2c;

    };}
