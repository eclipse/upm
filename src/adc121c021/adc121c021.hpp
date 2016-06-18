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
   * @brief ADC121C021 I2C Analog-to-Digital Converter library
   * @defgroup adc121c021 libupm-adc121c021
   * @ingroup seeed i2c electric
   */


  /**
   * Valid cycle times for the automatic conversion mode
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
   * @comname ADC121C021 Analog-to-Digital Converter
   * @altname Grove I2C ADC
   * @type electric
   * @man seeed
   * @web http://www.seeedstudio.com/depot/Grove-I2C-ADC-p-1580.html
   * @con i2c
   *
   * @brief API for the ADC121C021 I2C Analog-to-Digital Converter
   *
   * UPM module for the ADC121C021 12-bit analog-to-digital converter (ADC).
   * By constantly providing a reference voltage, this sensor helps
   * increase the accuracy of a value collected from an analog sensor.
   *
   * @image html adc121c021.jpg
   * @snippet adc121c021.cxx Interesting
   */
  class ADC121C021 {
  public:
    /**
     * ADC121C021 ADC constructor
     *
     * @param bus I2C bus to use
     * @param address Address for this sensor; default is 0x55
     * @param vref Reference voltage for this sensor; default is 3.0v
     */
    ADC121C021(int bus, uint8_t address = ADC121C021_DEFAULT_I2C_ADDR,
               float vref = ADC121C021_DEFAULT_VREF);

    /**
     * ADC121C021 destructor
     */
    ~ADC121C021();

    /**
     * Writes a byte value into the register
     *
     * @param reg Register location to write into
     * @param byte Byte to write
     * @return 0 (MRAA_SUCCESS) if successful; non-zero otherwise
     */
    mraa_result_t writeByte(uint8_t reg, uint8_t byte);

    /**
     * Writes a word value into the register
     *
     * @param reg Register location to write into
     * @param word Word to write
     * @return 0 (MRAA_SUCCESS) if successful; non-zero otherwise
     */
    mraa_result_t writeWord(uint8_t reg, uint16_t word);

    /**
     * Reads the byte value from the register
     *
     * @param reg Register location to read from
     * @return Value in the specified register
     */
    uint8_t readByte(uint8_t reg);

    /**
     * Reads the word value from the register
     *
     * @param reg Register location to read from
     * @return Value in the specified register
     */
    uint16_t readWord(uint8_t reg);

    /**
     * Reads the current value of conversion 
     *
     * @return Current value of conversion
     */
    uint16_t value();

    /**
     * Converts a supplied value to voltage based on the set vref
     *
     * @param val Value of conversion (from value())
     * @return Value of conversion in volts
     */
    float valueToVolts(uint16_t val);

    /**
     * Reads the current status of the alert flag.  If the flag is set, the
     * low or high alert indicators are set as appropriate, and
     * you can access these values with alertLowTriggered() or
     * alertHighTriggered().
     *
     * @return True if the alert flag is set
     */
    bool getAlertStatus();

    /**
     * Returns the current value of m_alertLow.  You must call
     * getAlertStatus() to update this value. 
     *
     * @return Current status of the alert low flag 
     */
    bool alertLowTriggered() { return m_alertLow; };

    /**
     * Returns the current value of m_alertHigh.  You must call
     * getAlertStatus() to update this value. 
     *
     * @return Current status of the alert high flag
     */
    bool alertHighTriggered() { return m_alertHigh; };

    /**
     * Clears the alert low and alert high flags. This also clears the
     * last stored alert values.
     */
    void clearAlertStatus();

    /**
     * Enables or disables the alert flag functionality.  If enabled,
     * when the measured value exceeds the low or high limits
     * configured, the alert flag is set. Use getAlertStatus()
     * to access these values.
     *
     * @param enable If true, enables the alert flag; otherwise, disables the
     * alert flag
     */
    void enableAlertFlag(bool enable);

    /**
     * Enables or disables the alert pin functionality.
     *
     * @param enable If true, enables the alert pin; otherwise, disables the
     * alert pin
     */
    void enableAlertPin(bool enable);

    /**
     * Enables or disables the alert hold functionality.  When the alert
     * hold is enabled, the alert status remains until manually
     * cleared via clearAlertStatus().  Otherwise, the alert self-clears
     * when the value moves into the defined limits if alerts
     * are enabled via enableAlertFlag().
     *
     * @param enable If true, enables the alert hold; otherwise, disables the
     * alert hold
     */
    void enableAlertHold(bool enable);

    /**
     * If the alert pin is enabled, defines the active
     * polarity of the pin in an alert condition. Enabling this sets
     * the pin to active high in an alert condition; otherwise,
     * active low is used.
     *
     * @param enable If true, the alert pin is active high; otherwise, active
     * low
     */
    void enableAlertPinPolarityHigh(bool enable);

    /**
     * Enables or disables the automatic conversion mode. When enabled, the
     * ADC samples and updates the conversion value independently.
     * It is disabled by default, so conversion is only done by
     * calling value(). 
     *
     * @param cycleTime Sets the cycle time for automatic conversion
     */
    void setAutomaticConversion(ADC121C021_CYCLE_TIME_T cycleTime);

    /**
     * Sets the alert low limit.  If alerts are enabled and the
     * measured conversion value is lower than the low limit, an alert is
     * triggered.
     *
     * @param limit Alert low limit
     * @return 0 (MRAA_SUCCESS) if successful; non-zero otherwise
     */
    mraa_result_t setAlertLowLimit(uint16_t limit);

    /**
     * Sets the alert high limit.  If alerts are enabled and the
     * measured conversion value is higher than the high limit, an alert is
     * triggered.
     *
     * @param limit Alert high limit
     * @return 0 (MRAA_SUCCESS) if successful; non-zero otherwise
     */
    mraa_result_t setAlertHighLimit(uint16_t limit);

    /**
     * Sets the hysteresis value.  If a high or low alert condition is
     * triggered, the conversion result must move within the high or
     * low limit by more than this value to clear the alert condition.
     * If the alert hold is set, the alert doesn't self-clear
     * regardless of this value.
     *
     * @param limit Hysteresis limit
     * @return 0 (MRAA_SUCCESS) if successful; non-zero otherwise
     */
    mraa_result_t setHysteresis(uint16_t limit);

    /**
     * Returns the highest conversion value recorded so far. This value
     * is only updated by the converter when the automatic conversion mode
     * is enabled.
     *
     * @return Highest conversion value
     */
    uint16_t getHighestConversion();

    /**
     * Returns the lowest conversion value recorded so far. This value
     * is only updated by the converter when the automatic conversion mode
     * is enabled.
     *
     * @return Lowest conversion value
     */
    uint16_t getLowestConversion();

    /**
     * Clears the highest conversion value recorded so far.
     *
     * @return 0 (MRAA_SUCCESS) if successful; non-zero otherwise
     */
    mraa_result_t clearHighestConversion();

    /**
     * Clears the lowest conversion value recorded so far.
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


