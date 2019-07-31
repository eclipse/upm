/*
 * Author: Lay, Kuan Loon <kuan.loon.lay@intel.com>
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
#include <mraa/iio.h>
#include <mraa/initio.hpp>

namespace upm
{
/**
 * @brief APDS9930 Digital Proximity and Ambient Light Sensor
 * @defgroup apds9930 libupm-apds9930
 * @ingroup Avago Technologies iio i2c proximity and ambient light sensor
 */

/**
 * @library apds9930
 * @sensor apds9930
 * @comname Digital Ambient Light and Proximity Sensor
 * @type light proximity
 * @man avago
 * @con iio i2c
 * @web https://www.broadcom.com/products/optical-sensors/integrated-ambient-light-proximity-sensors/apds-9930
 *
 * @brief APDS9930 Digital Proximity and Ambient Light Sensor
 *
 * This sensor provides digital ambient light sensing (ALS),
 * IR LED and a complete proximity detection system.
 *
 * @snippet apds9930.cxx Interesting
 */

class APDS9930
{
  public:
    /**
     * APDS-9930 digital proximity and ambient light sensor constructor
     *
     * @param device iio device number
     */
    APDS9930(int device);
    /**
     * Instantiates APDS9930 Digital Proximity and Ambient Light Sensor based on a given string.
     *
     * @param initStr string containing specific information for APDS9930 initialization.
     */
    APDS9930(std::string initStr);
    /**
     * APDS9930 destructor
     */
    ~APDS9930();
    /**
     * Gets the ambient luminance value from the sensor
     *
     * @return Ambient Luminance value
     */
    int getAmbient();
    /**
     * Gets the proximity value from the sensor
     *
     * @return Proximity value
     */
    int getProximity();
    /**
     * Enable proximity
     * @param enable state
     */
    bool enableProximity(bool enable);
    /**
     * Enable illuminance
     * @param enable state
     */
    bool enableIlluminance(bool enable);

  private:
    mraa_iio_context m_iio;
    mraa::MraaIo mraaIo;
};
}
