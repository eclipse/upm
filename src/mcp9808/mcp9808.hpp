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

#define MCP9808_REG_CONFIG  0x01
#define MCP9808_REG_AMBIENT_TEMP 0x05
#define MCP9808_REG_MANUF_ID 0x06
#define MCP9808_REG_DEVICE_ID 0x07
#define MCP9808_REG_RESOLUTION 0x08
#define MCP9808_CONFIG_SHUTDOWN 0x0001
#define MCP9808_CONFIG_CRITLOCKED 0x8000
#define MCP9808_CONFIG_WINLOCKED 0x4000
#define MCP9808_CONFIG_INTCLR 0x2000

namespace upm {
  /**
   * @brief MCP9808 precision temperature sensor library
   * @defgroup mcp9808 libupm-mcp9808
   * @ingroup adafruit i2c temp
   */
  /**
   * @library mcp9808
   * @sensor MCP9808
   * @comname MCP9808 Temperature Sensor
   * @type temp
   * @man adafruit
   * @con i2c
   * @web https://learn.adafruit.com/adafruit-mcp9808-precision-i2c-temperature-sensor-guide/overview
   * @web http://ww1.microchip.com/downloads/en/DeviceDoc/25095A.pdf
   *
   * @brief API for MCP9808 precision temprature sensor
   *
   * The MCP9808 digital temperature sensor converts temperatures between -20 deg C and +100 deg C
   * to a digital word with +/- 0.5 deg C (max.) accuracy. The MCP9808 comes with user-programmable
   * registers that provide flexibility for temperature sensing applications. The registers
   * allow user-selectable settings such as Shutdown or low-power modes and the specification
   * of temperature Event and Critical output boundaries. When the temperature changes beyond
   * the specified boundary limits, the MCP9808 outputs an Event signal. The user has the
   * option of setting the event output signal polarity as an active-low or active-high
   * comparator output for thermostat operation, or as temperature event interrupt output
   * for microprocessor-based systems. The event output can also be configured as a Critical
   * temperature output.
   *
   * Tested with Adafriut MCP9808 board.
   *
   * @image html mcp9808.jpg
   * @snippet mcp9808.cxx Interesting
   */
    class MCP9808 {

        public:
            /**
             * @enum MCP9808_REG
             * @brief uint8_t enum containing register addresses
             * used for setting temp thresholds for MCP9808
             *
             * @var MCP9808_REG::UPPER_TEMP = 0x02
             * @var MCP9808_REG::LOWER_TEMP = 0x03
             * @var MCP9808_REG::CRIT_TEMP = 0x04
             */
            typedef enum
            {
                 UPPER_TEMP = 0x02,
                 LOWER_TEMP = 0x03,
                 CRIT_TEMP = 0x04,
            } MCP9808_REG;

            /**
             * @enum MCP9808_RESOLUTION
             * @brief uint8_t enum containing the four possible
             * values for MCP9808 resolution register.
             *
             * @var MCP9808_RESOLUTION::RES_LOW = 0.5C
             * @var MCP9808_RESOLUTION::RES_MEDIUM = 0.25C
             * @var MCP9808_RESOLUTION::RES_HIGH = 0.125C
             * @var MCP9808_RESOLUTION::RES_PRECISION = (default) 0.0625C
             */
            typedef enum
            {
                RES_LOW = 0x00,
                RES_MEDIUM = 0x01,
                RES_HIGH = 0x02,
                RES_PRECISION = 0x03
            } MCP9808_RESOLUTION;

            /**
             * @enum MCP9808_CONFIG
             * @brief uint16_t enum containing alert and hysteresis options
             * for config register.
             *
             * @var MCP9808_CONFIG::ALERTSTAT - Alert Output Status bit
             *      0 = Alert output is not asserted by the device (power-up default)
             *      1 = Alert output is asserted as a comparator/Interrupt or critical
             *      temperature output
             * @var MCP9808_CONFIG::ALERTCTRL - Alert Output Control bit
             *      0 = Disabled (power-up default)
             *      1 = Enabled
             * @var MCP9808_CONFIG::ALERTSEL -  Alert Output Select bit
             *      0 = Alert output for TUPPER, TLOWER and TCRIT (power-up default)
             *      1 = TA > TCRIT only (TUPPER and TLOWER temperature boundaries are disabled)
             * @var MCP9808_CONFIG::ALERTPOL - Alert Output Polarity bit
             *      0 = Active-low (power-up default; pull-up resistor required)
             *      1 = Active-high
             * @var MCP9808_CONFIG::ALERTMODE - Alert Output Mode bit
             *      0 = Comparator output (power-up default)
             *      1 = Interrupt output
             * @var MCP9808_CONFIG::HYST_0 : 0 deg C
             * @var MCP9808_CONFIG::HYST_1_5 : +1.5 deg C
             * @var MCP9808_CONFIG::HYST_3_0 : +3.0 deg C
             * @var MCP9808_CONFIG::HYST_6_0 : +6.0 deg C
             */
            typedef enum
            {
                ALERTSTAT = 0x1000,
                ALERTCTRL = 0x0800,
                ALERTSEL = 0x0400,
                ALERTPOL = 0x0200,
                ALERTMODE = 0x0100,
                HYST_0 = 0x0000,
                HYST_1_5 = 0x0002,
                HYST_3_0 = 0x0004,
                HYST_6_0 = 0x0006
            } MCP9808_CONFIG;

            /**
             * MCP9808 constructor
             *
             * @param bus i2c bus the sensor is attached to.
             * @param address. Device address. Default is 0x18.
             */
            MCP9808 (int bus, uint8_t address = 0x18);

            /**
             * MCP9808 destructor
             */
            ~MCP9808 ();

            /**
             * Returns the name of the sensor
             */
            std::string name()
            {
                return m_name;
            }

            /**
             * Returns current temperature.
             */
            float getTemp(void);

            /**
             *  Will cause the devices to either sleep or wakeup.
             *
             *  @param sleep . Bool, default true to sleep. false to wake.
             */
             void shutDown(bool sleep = true);

            /**
             *  setMode - sets temperature reporting mode.
             *
             *  @param celsius. Default is true. If false all
             *  temps will be reported in fahrenheit.
             */
            void setMode(bool celsius = true)
            {
                m_celsius = celsius;
            }

            /**
             * Returns true if mode is celsius
             * False if fahrenheit.
             */
            bool isCelsius(void)
            {
                return m_celsius;
            }

            /**
             * Returns true if TCrit threshold has been crossed
             * Reflects the state of the bit based on the most recent
             * readTemp() operation.
             */
            bool isTcrit()
            {
                return m_tcrit;
            }

            /**
             * Returns true if TUpper threshold crossed
             * Reflects the state of the bit based on the most recent
             * readTemp() operation.
             */
            bool isTupper()
            {
                return m_tupper;
            }

            /**
             * Returns true if TLower threshold crossed.
             * Reflects the state of the bit based on the most recent
             * readTemp() operation.
             */
            bool isTlower()
            {
                return m_tlower;
            }

            /**
             * Sets the value of TCrit, TUpper,TLower registers.
             *
             * @praam reg - MCP9808_REG enum UPPER_TEMP, LOWER_TEMP
             * or CRIT_TEMP.
             * @param value - float value representing the set value
             */
            void setMonitorReg(MCP9808_REG reg, float value);

            /**
             * Returns the current value of TCrit, TUpper, TLower
             * registers.
             *
             * @param reg - MCP9808_REG enum UPPER_TEMP, LOWER_TEMP
             * or CRIT_TEMP.
             */
            float getMonitorReg(MCP9808_REG reg);

            /**
             * Clears the interrupt when ALERT_MODE is set to
             * interrupt output and temp threshold crossed.
             */
            void clearInterrupt(void);

            /**
             *  Sets alert mode. Can use values from MCP9808_CONFIG
             *  enum. Values can be combined.
             *
             *  @param command - a combination of options to set desired
             *  alert mode. See spcec sheet.
             */
            void setAlertMode(uint16_t command);

            /**
             * Clears Alert Mode -- sets all params to default.
             */
            void clearAlertMode(void);

            /**
             * Sets hysteresis value.
             *
             * @param MCP9808_CONFIG enum value HYST_0, HYST_1_5,
             *  HYST_3_0 or HYST_6_0
             */
            void setHysteresis(MCP9808_CONFIG value);

            /**
             * Returns hysteresis setting as a float value.
             */
            float getHysteresis();

            /**
             *  Sets resolution of temperature conversion.
             *
             *  @param value - MCP9808_RESOLUTION enum value.
             *  RES_LOW = +0.5 C
             *  RES_MEDIUM = +0.25 C
             *  RES_HIGH = +0.125 C
             *  RES_PRECISION = +0.0625 C (default).
             */
            void setResolution(MCP9808_RESOLUTION value);

            /**
             *  Returns float value representing the current
             *  resolution setting.
             */
            float getResolution();

            /**
             * Returns Manufacturer ID. Typically 0x0054;
             */
            uint16_t getManufacturer();

            /**
             * Returns device ID and revision. Typically 0x0400
             * With ID in the High byte.
             */
            uint16_t getDevicedId();

        private:
            std::string m_name;
            bool m_celsius;
            bool m_tcrit;
            bool m_tupper;
            bool m_tlower;

            mraa::I2c* i2c;

            float getTempValue(uint16_t value);
            void updateConfigRegister(uint16_t update, bool on = true);
            uint16_t swapWord(uint16_t value);
    };
}
