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
 * @brief KXCJK1013 Tri-axis Digital Accelerometer
 * @defgroup kxcjk1013 libupm-kxcjk1013
 * @ingroup Kionix iio i2c tri-axis digital accelerometer
 */

/**
 * @library kxcjk1013
 * @sensor kxcjk1013
 * @comname KXCJK1013 Tri-axis Digital Accelerometer
 * @type accelerometer
 * @man Kionix
 * @con iio i2c
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
     * @param iio device number
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
     * @param interrupt channel
     * @param fptr Pointer to a function to be called on interrupt
     * @param arg Pointer to an object to be supplied as an
     * argument to the ISR.
     */
    void installISR(void (*isr)(char*), void* arg);

    /**
     * Extract the channel value based on channel type
     * @param input Channel data
     * @param chan MRAA iio-layer channel info
     */
    int64_t getChannelValue(unsigned char* input, mraa_iio_channel* chan);

    /**
     * Enable trigger buffer
     * @param trigger buffer length in integer
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
     * @param sampling frequency in float
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
