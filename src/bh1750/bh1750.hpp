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

#include <string>
#include <iostream>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <interfaces/iLight.hpp>

#include "bh1750.h"
#include "mraa/initio.hpp"

namespace upm {
    /**
     * @brief BH1750 Light Sensor
     * @defgroup bh1750 libupm-bh1750
     * @ingroup rohm dfrobot i2c
     */

    /**
     * @library bh1750
     * @sensor bh1750
     * @comname Digital Light Sensor
     * @type light
     * @man rohm dfrobot
     * @con i2c
     * @web http://www.dfrobot.com/index.php?route=product/product&product_id=531
     *
     * @brief UPM C API for the DFRobot I2C BH1750 Light Sensor
     *
     * This driver was developed with the DFRobot Light Sensor based on
     * the BH1750.  It has a sensitivity of .5 10 65535 Lux.  It supports
     * voltages from 3-5vdc and is connected via I2C.
     *
     * @snippet bh1750.cxx Interesting
     */

  class BH1750 : virtual public iLight {
  public:

    /**
     * BH1750 object constructor (Analog mode)
     *
     * @param bus The I2C bus to use
     * @param addr The I2C address of the device
     * @param mode The mode to start operation under.  One of the
     * BH1750_OPMODES_T values.  The default is the highest precision,
     * lowest power mode.
     */
    BH1750(int bus=BH1750_DEFAULT_I2C_BUS, int addr=BH1750_DEFAULT_I2C_ADDR,
           BH1750_OPMODES_T mode=BH1750_OPMODE_H2_ONCE);

    /**
     * Instantiates BH1750 Light Sensor object based on a given string.
     *
     * @param initStr string containing specific information for BH1750 initialization.
     */
    BH1750(std::string initStr);

    /**
     * BH1750 object destructor
     */
    ~BH1750();

    /**
     * Reset the device.  This doesn't really have much purpose.  The
     * device must be powered up for this command to work.  In
     * addition, this command will simply clear the measurement
     * register to 0.
     */
    void reset();

    /**
     * Get the measured light intensity in Lux.
     *
     * @return The measured light intensity in Lux.
     */
    float getLux();

    /**
     * Gets the luminance value from the sensor
     *
     * @return The measured light intensity value in Lux
     */
    virtual float getLuminance();

    /**
     * Power up the device.
     */
    void powerUp();

    /**
     * Power down the device.
     */
    void powerDown();

    /**
     * Setup the device to match the selected operating mode.
     *
     * @param mode operating mode, one of the BH1750_OPMODES_T values
     */
    void setOpmode(BH1750_OPMODES_T mode);

  protected:
    // bh1750 device context
    bh1750_context m_bh1750;
    mraa::MraaIo mraaIo;

    /**
     * Sends a command to the device via I2C.
     *
     * @param cmd The command to write, one of the BH1750_CMD* values
     */
    void sendCommand(uint8_t cmd);

    /**
     * Read the 2 result bytes from the device via I2C.
     *
     * @return Data read in from the device as a uint16_t
     */
    uint16_t readData();

  private:
    /* Disable implicit copy and assignment operators */
    BH1750(const BH1750&) = delete;
    BH1750 &operator=(const BH1750&) = delete;
  };
}
