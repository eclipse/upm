/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
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

#include <mraa/common.hpp>
#include <mraa/uart.hpp>

#define NLGPIO16_DEFAULT_UART_DEV "/dev/ttyACM0"

// ADC analog ref voltage is 3.3v
#define ADC_AREF 3.3

// 10-bit precision (0-1023)
#define ADC_PRECISION 10

namespace upm {
    /**
     * @brief NLGPIO16 module
     * @defgroup nlgpio16 libupm-nlgpio16
     * @ingroup sparkfun uart gpio ainput
     */

    /**
     * @library nlgpio16
     * @sensor nlgpio16
     * @comname NLGPIO16 16 channel USB GPIO Module
     * @type other
     * @man numatolabs
     * @con uart
     * @web http://numato.com/16-channel-usb-gpio-module-with-analog-inputs/
     *
     * @brief API for the NLGPIO16 16 channel USB GPIO Module
     *
     * The NLGPIO16 is a USB adapter providing access to 16 GPIO's, 7
     * of which can be used as analog inputs.  The GPIO's are 3.3v
     * only.  An external power supply can be connected to provide
     * more current if the need arises.
     *
     * It is recommended to use a series resistor with the GPIO/ADC
     * pins when interfacing with other circuits. In output mode,
     * GPIOs can source up to 8mA (gpio8-gpio15). So no additional
     * circuitry is needed to drive regular LEDs. A 470 Ohms series
     * resistor is recommended for current limiting when connecting an
     * LED to a GPIO. In contrast to GPIOs, analog inputs can read
     * voltages at any level between 0 to 3.3V volts. It is
     * recommended to use a series resistor to protect the input from
     * stray voltages and spikes. The internal Analog to Digital
     * converter supports 10 bits resolution which is adequate for
     * most applications.
     *
     * Maximum IO source/sink current on GPIO 0-7 is 2mA
     * Maximum IO source/sink current on GPIO 8-15 is 8mA
     *
     * @snippet nlgpio16.cxx Interesting
     */

  class NLGPIO16 {
  public:

    /**
     * NLGPIO16 object constructor
     *
     * @param uart UART device path to use. Default is /dev/ttyACM0.
     */
    NLGPIO16(std::string uart=NLGPIO16_DEFAULT_UART_DEV);

    /**
     * NLGPIO16 object destructor
     */
    ~NLGPIO16();

    /**
     * Get the version of the device
     *
     * @return String containing device revision
     */
    std::string getVersion();

    /**
     * Get the device ID.  The device ID is an 8 character
     * alpha-numeric string.
     *
     * @return String containing device ID
     */
    std::string getID();

    /**
     * Set the device ID.  The device ID is an 8 character
     * alpha-numeric string.  The supplied ID must be exactly 8
     * characters in length.
     *
     * @param id String containing a new 8 character device ID
     */
    void setID(std::string id);

    /**
     * Set a gpio output to the HIGH (1) state.  The gpio is
     * automatically set as an output when this call is made,
     * regardless of it's previous mode.
     *
     * @param gpio The gpio to set.  Valid values are between 0-15
     */
    void gpioSet(int gpio);

    /**
     * Set a gpio output to the LOW (0) state.  The gpio is
     * automatically set as an output when this call is made,
     * regardless of it's previous mode.
     *
     * @param gpio The gpio to clear.  Valid values are between 0-15
     */
    void gpioClear(int gpio);

    /**
     * Read the state of a gpio.  The gpio is automatically set as an
     * input when this call is made, regardless of it's previous
     * mode.
     *
     * @param gpio The gpio to read.  Valid values are between 0-15
     * @return true if the gpio is in the HIGH state, false otherwise
     */
    bool gpioRead(int gpio);

    /**
     * Read the state of all gpios.  The returned integer is a bitmask
     * of all 16 gpios, where a 0 bit means the gpio is in the LOW
     * state, whereas a 1 bit means the gpio is in a HIGH state.
     *
     * @return bitmask of the state of all 16 gpios. The LSB is gpio0.
     */
    unsigned int gpioReadAll();

    /**
     * Set a mask for selectively updating multiple gpios with the
     * gpioIODir() and gpioWriteAll() methods.  Each bit set in the 16
     * bit argument (LSB = gpio0) represents whether the two previously
     * mentioned methods will act on a given gpio or not.  A 0 in a
     * given bit position will cause any update to that gpio via
     * gpioIODir() and gpioWriteAll() to be ignored, while a 1 bit
     * enables that gpio to be affected by those two methods.
     *
     * @param mask A bitmask of the 16 gpios affected by gpioIODir() and
     * gpioWriteAll()
     */
    void gpioSetIOMask(int mask);

    /**
     * Set the driection mode (input or output) for all gpios enabled
     * by gpioSetIOMask().  A 0 in a given bit position (LSB = gpio0)
     * configures the gpio as an output, and a 1 bit configures the
     * gpio as an input.  Only the gpios enabled by gpioSetMask() are
     * affected by this call.
     *
     * @param mask A bitmask of the 16 gpios whose direction mode is
     * to be set
     */
    void gpioSetIODir(int mask);

    /**
     * Write all enabled gpios (set via gpioSetIOMask()) to a given
     * value.  A 1 bit (LSB = gpio0) sets the given output to HIGH, a
     * zero sets the given output to LOW.  Only the gpios enabled by
     * gpioSetMask() are affected by this call.
     *
     * @param mask The values to set for the 16 gpios (LSB = gpio0)
     */
    void gpioWriteAll(int mask);

    /**
     * Read the raw analog input value present at the given gpio.  The
     * gpio is switched to analog input mode by this call, regardless of
     * any previous mode.  The returned value will be a number between
     * 0-1023 (10 bit resolution). Only the first 7 gpios (0-6) can be
     * used for analog input.
     *
     * @param adc The gpio number to read (0-6)
     * @return The raw integer value from the ADC (0-1023)
     */
    int analogReadValue(int adc);

    /**
     * Read the raw analog input value present at the given gpio and
     * return the corresponding voltage value at the pin.  The gpio is
     * switched to analog input mode by this call, regardless of any
     * previous mode.  The returned value will be a number between
     * 0.0-3.3, depending on the voltage present at the pin.  Only the
     * first 7 gpios (0-6) can be used for analog input.
     *
     * @param adc The gpio number to read (0-6)
     * @return The voltage present at the pin
     */
    float analogReadVolts(int adc);


  protected:
    mraa::Uart m_uart;

    // UART helpers
    bool dataAvailable(unsigned int millis);
    std::string readStr(int len);
    int writeStr(std::string data);

    // does most of the work for sending commands and getting responses
    std::string sendCommand(std::string cmd);

  private:
  };
}
