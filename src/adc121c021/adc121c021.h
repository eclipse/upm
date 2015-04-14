/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
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
#include <mraa/i2c.h>

#define ADC121C021_I2C_BUS 0
#define ADC121C021_DEFAULT_I2C_ADDR 0x55

#define ADC121C021_RESOLUTION  4096 // 12 bits

/**
 * ADC121C021 registers
 */

#define ADC121C021_REG_RESULT          0x00
#define ADC121C021_REG_ALERT_STATUS    0x01
#define ADC121C021_REG_CONFIG          0x02
#define ADC121C021_REG_ALERT_LIM_UNDER 0x03
#define ADC121C021_REG_ALERT_LIM_OVER  0x04
#define ADC121C021_REG_ALERT_HYS       0x05
#define ADC121C021_REG_LOWEST_CONV     0x06
#define ADC121C021_REG_HIGHEST_CONV    0x07

// For the Grove I2C ADC
#define ADC121C021_DEFAULT_VREF        3.0

namespace upm {
  /**
   * @brief ADC121C021 I2C Analog Digital Converter library
   * @defgroup adc121c021 libupm-adc121c021
   * @ingroup seeed i2c electric
   */


  /**
   * valid cycle times for automatic conversion mode
   */

  typedef enum { ADC121C021_CYCLE_NONE = 0,  // disabled
                 ADC121C021_CYCLE_32   = 1,  // 27 ksps
                 ADC121C021_CYCLE_64   = 2,  // 13.5
                 ADC121C021_CYCLE_128  = 3,  // 6.7
                 ADC121C021_CYCLE_256  = 4,  // 3.4
                 ADC121C021_CYCLE_512  = 5,  // 1.7
                 ADC121C021_CYCLE_1024 = 6,  // 0.9
                 ADC121C021_CYCLE_2048 = 7   // 0.4
  } ADC121C021_CYCLE_TIME_T;

  /**
   * @library adc121c021
   * @sensor adc121c021
   * @comname ADC121C021 Analog Digital Converter
   * @type electric
   * @man seeed
   * @web http://www.seeedstudio.com/depot/Grove-I2C-ADC-p-1580.html
   * @con i2c
   *
   * @brief C++ API for the ADC121C021 I2C ADC
   *
   * UPM module for the ADC121C021 12 bit Analog to Digital (ADC) converter.
   * By providing a constant reference voltage, this sensor helps
   * to increase the accuracy of a value collected from an analog sensor.
   *
   * @snippet adc121c021.cxx Interesting
   */
  class ADC121C021 {
  public:
    /**
     * adc121c021 ADC constructor
     *
     * @param bus i2c bus to use
     * @param address the address for this sensor; default is 0x55
     * @param vref reference voltage for this sensor; default is 3.0
     */
    ADC121C021(int bus, uint8_t address = ADC121C021_DEFAULT_I2C_ADDR,
               float vref = ADC121C021_DEFAULT_VREF);

    /**
     * ADC121C021 Destructor
     */
    ~ADC121C021();

    /**
     * Write byte value into register
     *
     * @param reg register location to write into
     * @param byte byte to write
     * @return 0 (MRAA_SUCCESS) if successful; non-zero otherwise
     */
    mraa_result_t writeByte(uint8_t reg, uint8_t byte);

    /**
     * Write word value into register
     *
     * @param reg register location to write into
     * @param word word to write
     * @return 0 (MRAA_SUCCESS) if successful; non-zero otherwise
     */
    mraa_result_t writeWord(uint8_t reg, uint16_t word);

    /**
     * Read byte value from register
     *
     * @param reg register location to read from
     * @return value at specified register
     */
    uint8_t readByte(uint8_t reg);

    /**
     * Read word value from register
     *
     * @param reg register location to read from
     * @return value at specified register
     */
    uint16_t readWord(uint8_t reg);

    /**
     * Read current value of conversion
     *
     * @return current conversion value
     */
    uint16_t value();

    /**
     * Convert a supplied value to voltage based on set VREF
     *
     * @param val value of conversion (from value())
     * @return conversion value in volts
     */
    float valueToVolts(uint16_t val);

    /**
     * Read current status of the alert flag.  If the flag is set, the
     * lower or upper alert indicators will be set as appropriate, and
     * you can access these values with alertLowTriggered() or
     * alertHighTriggered().
     *
     * @return true if the alert flag is set
     */
    bool getAlertStatus();

    /**
     * Return the current value of m_alertLow.  You must call
     * getAlertStatus() to update this value. 
     *
     * @return current alert low status
     */
    bool alertLowTriggered() { return m_alertLow; };

    /**
     * Return the current value of m_alertHigh.  You must call
     * getAlertStatus() to update this value. 
     *
     * @return current alert high status
     */
    bool alertHighTriggered() { return m_alertHigh; };

    /**
     * Clear the alert low and high flags.  This will also clear the
     * last stored alert values.
     */
    void clearAlertStatus();

    /**
     * Enable or disable the Alert Flag functionality.  If enabled,
     * then when the measured value exceeds the low or high limits
     * configured, the alert flag will be set.  Use getAlertStatus()
     * to access these values.
     *
     * @param enable if true, enables Alert Flag; otherwise, disables Alert Flag
     */
    void enableAlertFlag(bool enable);

    /**
     * Enable or disable the Alert Pin functionality.
     *
     * @param enable if true, enables Alert Pin; otherwise, disables Alert Pin
     */
    void enableAlertPin(bool enable);

    /**
     * Enable or disable the Alert Hold functionality.  When Alert
     * Hold is enabled, the alert status remains until manually
     * cleared via clearAlertStatus().  Otherwise, the alert will self
     * clear when the value moves into the defined limits if alerts
     * are enabled via enableAlertFlag().
     *
     * @param enable if true, enables Alert Hold; otherwise, disables Alert Hold
     */
    void enableAlertHold(bool enable);

    /**
     * If the Alert Pin fnctionality is enabled, define the active
     * polarity of the pin in an alert condition.  Enabling this sets
     * the pin to active high in an alert condition, otherwise an
     * active low is used.
     *
     * @param enable if true, Alert Pin is active high, else active low
     */
    void enableAlertPinPolarityHigh(bool enable);

    /**
     * Enable or disable Automatic Conversion mode.  When enabled, the
     * ADC will sample and update the conversion value independently.
     * This is disabled by default, and a conversion is only done by
     * calling value(). 
     *
     * @param cycleTime set the Cycle Time for automatic conversion
     */
    void setAutomaticConversion(ADC121C021_CYCLE_TIME_T cycleTime);

    /**
     * Set the Alert Low Limit.  If Alerts are enabled and the
     * measured conversion value is lower than this, an alert will be
     * triggered.
     *
     * @param limit the Low Alert Limit
     * @return 0 (MRAA_SUCCESS) if successful; non-zero otherwise
     */
    mraa_result_t setAlertLowLimit(uint16_t limit);

    /**
     * Set the Alert High Limit.  If Alerts are enabled and the
     * measured conversion value is higher than this, an alert will be
     * triggered.
     *
     * @param limit the High Alert Limit
     * @return 0 (MRAA_SUCCESS) if successful; non-zero otherwise
     */
    mraa_result_t setAlertHighLimit(uint16_t limit);

    /**
     * Set the Hysteresis value.  If a high or low alert condition is
     * triggered, the conversion result must move within the high or
     * low limit by more than this value to clear the alert condition.
     * If the Alert Hold bit is set, then the alert will not self
     * clear regardless of this value.
     *
     * @param limit Hysteresis Limit
     * @return 0 (MRAA_SUCCESS) if successful; non-zero otherwise
     */
    mraa_result_t setHysteresis(uint16_t limit);

    /**
     * Return the Highest Conversion value sampled so far.  This value
     * is only updated by the converter when automatic conversion mode
     * is enabled.
     *
     * @return the highest conversion value recorded
     */
    uint16_t getHighestConversion();

    /**
     * Return the Lowest Conversion value sampled so far.  This value
     * is only updated by the converter when automatic conversion mode
     * is enabled.
     *
     * @return the lowest conversion value recorded
     */
    uint16_t getLowestConversion();

    /**
     * Clear the Highest Conversion value sampled so far.
     *
     * @return 0 (MRAA_SUCCESS) if successful; non-zero otherwise
     */
    mraa_result_t clearHighestConversion();

    /**
     * Clear the Lowest Conversion value sampled so far.
     *
     * @return 0 (MRAA_SUCCESS) if successful; non-zero otherwise
     */
    mraa_result_t clearLowestConversion();

  private:
    mraa_i2c_context m_i2c;
    uint8_t m_addr;
    float m_vref;
    bool m_alertLow;
    bool m_alertHigh;
  };
}


