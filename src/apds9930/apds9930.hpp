/*
 * Author: Lay, Kuan Loon <kuan.loon.lay@intel.com>
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

#include <string>
#include <mraa/iio.h>

namespace upm
{
/**
 * @brief APDS9930 Digital Proximity and Ambient Light Sensor library
 * @defgroup apds9930 libupm-apds9930
 * @ingroup Avago Technologies iio i2c proximity and ambient light sensor
 */

/**
 * @library apds9930
 * @sensor apds9930
 * @comname APDS9930 Digital Proximity and Ambient Light Sensor
 * @type light proximity
 * @man Avago Technologies
 * @con iio i2c
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
     * @param iio device number
     */
    APDS9930(int device);
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
};
}
