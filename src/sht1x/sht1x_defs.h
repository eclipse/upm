/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
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
