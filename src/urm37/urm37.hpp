/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015-2016 Intel Corporation.
 *
 * Thanks to Adafruit for supplying a google translated version of the
 * Chinese datasheet and some clues in their code.
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
#include <iostream>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "urm37.h"

namespace upm {
    /**
     * @brief DFRobot URM37 Ultrasonic Ranger
     * @defgroup urm37 libupm-urm37
     * @ingroup dfrobot uart gpio ainput sound
     */

    /**
     * @library urm37
     * @sensor urm37
     * @comname DFRobot URM37 Ultrasonic Ranger
     * @type sound
     * @man dfrobot
     * @con uart ainput gpio
     * @web http://www.dfrobot.com/index.php?route=product/product&product_id=53
     *
     * @brief API for the DFRobot URM37 Ultrasonic Ranger
     *
     * The driver was tested with the DFRobot URM37 Ultrasonic Ranger,
     * V4.  It has a range of between 5 and 500 centimeters (cm).  It
     * supports both analog distance measurement, and UART based
     * temperature and distance measurements.  This driver does not
     * support PWM measurement mode.
     *
     * For UART operation, the only supported baud rate is 9600.  In
     * addition, you must ensure that the UART TX/RX pins are
     * configured for TTL operation (the factory default) rather than
     * RS232 operation, or permanent damage to your URM37 and/or MCU
     * will result.  On power up, the LED indicator will blink one
     * long pulse, followed by one short pulse to indicate TTL
     * operation.  See the DFRobot wiki for more information:
     *
     * (https://www.dfrobot.com/wiki/index.php?title=URM37_V4.0_Ultrasonic_Sensor_%28SKU:SEN0001%29)
     *
     * @image html urm37.jpg
     * An example using analog mode
     * @snippet urm37.cxx Interesting
     * An example using UART mode
     * @snippet urm37-uart.cxx Interesting
     */

  class URM37 {
  public:

    /**
     * URM37 object constructor (Analog mode)
     *
     * @param aPin Analog pin to use
     * @param resetPin GPIO pin to use for reset
     * @param triggerPin GPIO pin to use for triggering a distance measurement
     * @param aref The analog reference voltage, default 5.0
     */
    URM37(int aPin, int resetPin, int triggerPin, float aref=5.0);

    /**
     * URM37 object constructor (UART mode)
     *
     * @param uart Default UART to use (0 or 1).
     * @param resetPin GPIO pin to use for reset
     */
    URM37(int uart, int resetPin);

    /**
     * URM37 object destructor
     */
    ~URM37();

    /**
     * Reset the device.  This will take approximately 3 seconds to
     * complete.
     *
     */
    void reset();

    /**
     * Get the distance measurement.  A return value of 65535.0
     * in UART mode indicates an invalid measurement.
     *
     * @param degrees in UART mode, this specifies the degrees to turn
     * an attached PWM servo connected to the MOTO output on the
     * URM37.  Default is 0.  Valid values are 0-270.  This option is
     * ignored in analog mode.
     * @return The measured distance in cm
     */
    float getDistance(int degrees=0);

    /**
     * Get the temperature measurement.  This is only valid in UART mode.
     *
     * @return The measured temperature in degrees C
     */
    float getTemperature();

    /**
     * In UART mode only, read a value from the EEPROM and return it.
     *
     * @param addr The address in the EEPROM to read.  Valid values
     * are between 0x00-0x04.
     * @return The EEPROM value at addr
     */
    uint8_t readEEPROM(uint8_t addr);

    /**
     * In UART mode only, write a value into an address on the EEPROM.
     *
     * @param addr The address in the EEPROM to write.  Valid values
     * are between 0x00-0x04.
     * @param value The value to write
     * @return The EEPROM value at addr
     */
    void writeEEPROM(uint8_t addr, uint8_t value);

  protected:
    // urm37 device context
    urm37_context m_urm37;

  private:
  };
}


