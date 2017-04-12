/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
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

#ifdef __cplusplus
extern "C" {
#endif


  // SHT1X commands.  The first 3 msb's are the address, which are
  // always 0.  The following 5 bits are the actual command.
  typedef enum {
    SHT1X_CMD_MEAS_TEMPERATURE            = 0x03,
    SHT1X_CMD_MEAS_HUMIDITY               = 0x05,
    SHT1X_CMD_WRITE_STATUS                = 0x06,
    SHT1X_CMD_READ_STATUS                 = 0x07,
    SHT1X_CMD_SOFT_RESET                  = 0x1e
  } SHT1X_CMD_T;

  // status register bits
  typedef enum {
    SHT1X_STATUS_RESOLUTION_LOW           = 0x01, // 0=12b RH/14b temp (dflt)
    SHT1X_STATUS_NO_RELOAD_FROM_OTP       = 0x02,
    SHT1X_STATUS_HEATER_EN                = 0x04,

    // 0x08-0x20 reserved

    SHT1X_STATUS_LOW_VOLT                 = 0x40 // low battery

    // 0x80 reserved
  } SHT1X_STATUS_BITS_T;

  // The Vdd voltage can affect the temperature coefficients, so we
  // provide a way to indicate the closest voltage and set up the
  // compensation accordingly.
  typedef enum {
    SHT1X_VOLTS_5                         = 0, // 5 volts
    SHT1X_VOLTS_4                         = 1,
    SHT1X_VOLTS_3_5                       = 2, // 3.5v
    SHT1X_VOLTS_3                         = 3,
    SHT1X_VOLTS_2_5                       = 4
  } SHT1X_VOLTS_T;

#ifdef __cplusplus
}
#endif
