/*
 * Author: Jon Trulson <jtrulson@ics.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2016 Intel Corporation.
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

#ifndef URM37_H_
#define URM37_H_

#include <stdint.h>
#include "upm.h"
#include "mraa/aio.h"
#include "mraa/gpio.h"
#include "mraa/uart.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file urm37.h
 * @library urm37
 * @brief C API for the URM37 Ultrasonic Ranger
 *
 * An example using analog mode
 * @include urm37.c Interesting
 * An example using UART mode
 * @include urm37-uart.c Interesting
 */

/**
 * device context
 */
typedef struct _urm37_context {
  mraa_aio_context    aio;
  mraa_gpio_context   gpio_reset;
  mraa_gpio_context   gpio_trigger;
  mraa_uart_context   uart;

  bool                is_analog_mode;

  float               a_ref;
  float               a_res;
} *urm37_context;

/**
 * URM37 Initializer
 *
 * @param a_pin Analog pin to use. Ignored in UART mode.
 * @param reset_pin GPIO pin to use for reset
 * @param trigger_pin GPIO pin to use for triggering a distance
 * measurement. Ignored in UART mode.
 * @param a_ref The analog reference voltage. Ignored in UART mode.
 * @param uart Default UART to use (0 or 1). Ignored in analog mode.
 * @param mode true for analog mode, false otherwise.
 */
urm37_context urm37_init(uint8_t a_pin, uint8_t reset_pin,
                         uint8_t trigger_pin, float a_ref, 
                         uint8_t uart, bool analog_mode);

/**
 * URM37 sensor close function
 */
void urm37_close(urm37_context dev);

/**
 * Reset the device.  This will take approximately 3 seconds to
 * complete.
 *
 * @param dev sensor context
 */
upm_result_t urm37_reset(urm37_context dev);

/**
 * Get the distance measurement.  A return value of 65535.0
 * in UART mode indicates an invalid measurement.
 *
 * @param dev sensor context
 * @param distance A pointer to a float that will contain the distance
 * in CM if the measurement is successful.
 * @param degrees In UART mode, this specifies the degrees to turn an
 * attached PWM servo connected to the MOTO output on the URM37.
 * Valid values are 0-270.  This option is ignored in analog mode.  If
 * you are not using this functionality, just pass 0.
 * @return UPM status code
 */
upm_result_t urm37_get_distance(urm37_context dev, float *distance,
                                int degrees);

/**
 * Get the temperature measurement.  This is only valid in UART mode.
 *
 * @param dev sensor context
 * @param temperature A float pointer containing the measured
 * temperature in degrees C
 * @return UPM status code
 *
 */
upm_result_t urm37_get_temperature(urm37_context dev, float* temperature);

/**
 * In UART mode only, read a value from the EEPROM and return it.
 *
 * @param dev sensor context
 * @param addr The address in the EEPROM to read.  Valid values
 * are between 0x00-0x04.
 * @param value A pointer containing the returned value.
 * @return UPM status code
 */
upm_result_t urm37_read_eeprom(urm37_context dev, uint8_t addr, uint8_t* value);

/**
 * In UART mode only, write a value into an address on the EEPROM.
 *
 * @param dev sensor context
 * @param addr The address in the EEPROM to write.  Valid values
 * are between 0x00-0x04.
 * @param value The value to write
 * @return UPM status code
 */
upm_result_t urm37_write_eeprom(urm37_context dev, uint8_t addr, uint8_t value);

/**
 * In UART mode only, send a 4-byte command, and return a 4-byte response.
 *
 * @param dev sensor context
 * @param cmd A 4-byte command to transmit
 * @param response The 4-byte response
 * @return UPM response code (success, failure, or timeout)
 */
upm_result_t urm37_send_command(urm37_context dev, char* cmd, char* response);

#ifdef __cplusplus
}
#endif

#endif /* URM37_H_ */
