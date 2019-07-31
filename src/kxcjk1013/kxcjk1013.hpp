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

namespace upm
{
/**
 * @brief KXCJK1013 Tri-axis Digital Accelerometer
 * @defgroup kxcjk1013 libupm-kxcjk1013
 * @ingroup Kionix iio i2c tri-axis digital accelerometer
 */

/**
 * @library kxcjk1013
 * @sensor kxcjk1013
 * @comname Tri-axis Digital Accelerometer
 * @type accelerometer
 * @man kionix
 * @con iio i2c
 * @web http://kionixfs.kionix.com/en/datasheet/KXCJK-1013%20Specifications%20Rev%202.pdf
 *
 * @brief KXCJK1013 Tri-axis Digital Accelerometer API
 *
 * The KXCJK is a tri-axis +/-2g, +/-4g or +/-8g silicon micromachined
 * accelerometer.
 *
 * @snippet kxcjk1013.cxx Interesting
 */

class KXCJK1013
{
  public:
    /**
     * KXCJK1013 Tri-axis Digital Accelerometer
     *
     * @param device iio device number
     */
    KXCJK1013(int device);

    /**
     * KXCJK1013 destructor
     */
    ~KXCJK1013();

    /**
     * Installs an interrupt service routine (ISR) to be called when
     * an interrupt occurs
     *
     * @param isr Pointer to a function to be called on interrupt
     * @param arg Pointer to an object to be supplied as an
     * argument to the ISR.
     */
    void installISR(void (*isr)(char*, void*), void* arg);

    /**
     * Extract the channel value based on channel type
     * @param input Channel data
     * @param chan MRAA iio-layer channel info
     */
    int64_t getChannelValue(unsigned char* input, mraa_iio_channel* chan);

    /**
     * Enable trigger buffer
     * @param length buffer length in integer
     */
    bool enableBuffer(int length);

    /**
     * Disable trigger buffer
     */
    bool disableBuffer();

    /**
     * Set scale
     * @param scale in float
     * Available scales are 0.009582(2g), 0.019163(4g), and 0.038326(8g)
     * Default scale is 0.019163
     */
    bool setScale(const float scale);

    /**
     * Set sampling frequency
     * @param sampling_frequency sampling frequency in float
     * Available sampling frequency are 0.781000, 1.563000, 3.125000, 6.250000, 12.500000, 25, 50,
     * 100, 200, 400, 800, and 1600
     * Default sampling frequency is 25
     */
    bool setSamplingFrequency(const float sampling_frequency);

    /**
     * Enable 3 axis scan element
     */
    bool enable3AxisChannel();

    /**
     * Process enabled channel buffer and return x, y, z axis
     * @param data Enabled channel data, 6 bytes, each axis 2 bytes
     * @param x X-Axis
     * @param y Y-Axis
     * @param z Z-Axis
     */
    void extract3Axis(char* data, float* x, float* y, float* z);

  private:
    mraa_iio_context m_iio;
    int m_iio_device_num;
    bool m_mount_matrix_exist; // is mount matrix exist
    float m_mount_matrix[9];   // mount matrix
    float m_scale;             // accelerometer data scale
};
}
