/*
 * Author: Lay, Kuan Loon <kuan.loon.lay@intel.com>
 * Copyright (c) 2015 Intel Corporation.
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
 *
 * Thanks to https://github.com/01org/android-iio-sensors-hal for gyroscope
 * calibration and denoise algorithm.
 */
#pragma once

#include <string>
#include <mraa/iio.h>

namespace upm
{
/**
 * @brief L3GD20 Tri-axis Digital Gyroscope
 * @defgroup l3gd20 libupm-l3gd20
 * @ingroup STMicroelectronics iio i2c tri-axis digital gyroscope
 */

/**
 * @library l3gd20
 * @sensor l3gd20
 * @comname L3GD20 Tri-axis Digital Gyroscope
 * @type gyroscope
 * @man STMicroelectronics
 * @con iio i2c
 *
 * @brief L3GD20 Tri-axis Digital Gyroscope API
 *
 * The L3GD20 The L3GD20 is a low-power three-axis angular rate sensor.
 *
 * @snippet l3gd20.cxx Interesting
 */

class L3GD20
{
  public:
    typedef struct {
        float bias_x, bias_y, bias_z;
        int count;
        float min_x, min_y, min_z;
        float max_x, max_y, max_z;
    } gyro_cal_t;

    typedef struct {
        float* buff;
        unsigned int idx;
        unsigned int count;
        unsigned int sample_size;
    } filter_median_t;
    /**
     * L3GD20 Tri-axis Digital Gyroscope
     *
     * @param iio device number
     */
    L3GD20(int device);

    /**
     * L3GD20 destructor
     */
    ~L3GD20();

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
     * @param trigger buffer length in string
     */
    bool enableBuffer(int length);

    /**
     * Disable trigger buffer
     */
    bool disableBuffer();

    /**
     * Set scale
     * @param scale in string
     */
    bool setScale(const float scale);

    /**
     * Set sampling frequency
     * @param sampling frequency  in string
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
    bool extract3Axis(char* data, float* x, float* y, float* z);

    /**
     * Reset calibration data and start collect calibration data again
     */
    void initCalibrate();

    /**
     * Get calibrated status, return true if calibrate successfully
     */
    bool getCalibratedStatus();

    /**
     * Get calibrated data
     */
    void getCalibratedData(float* bias_x, float* bias_y, float* bias_z);

    /**
     * Load calibrated data
     */
    void loadCalibratedData(float bias_x, float bias_y, float bias_z);

    /**
     * Calibrate gyro
     * @param x X-Axis
     * @param y Y-Axis
     * @param z Z-Axis
     */
    bool gyroCollect(float x, float y, float z);

    /**
     * Denoise gyro
     * @param x X-Axis
     * @param y Y-Axis
     * @param z Z-Axis
     */
    void gyroDenoiseMedian(float* x, float* y, float* z);

    /**
     * median algorithm
     * @param queue
     * @param size
     */
    float median(float* queue, unsigned int size);

    /**
     * partition algorithm
     * @param list
     * @param left
     * @param right
     * @param pivot_index
     */
    unsigned int
    partition(float* list, unsigned int left, unsigned int right, unsigned int pivot_index);

    /**
     * Clamp Gyro Readings to Zero
     * @param x X-Axis
     * @param y Y-Axis
     * @param z Z-Axis
     */
    void clampGyroReadingsToZero(float* x, float* y, float* z);

  private:
    mraa_iio_context m_iio;
    int m_iio_device_num;
    bool m_mount_matrix_exist; // is mount matrix exist
    float m_mount_matrix[9];   // mount matrix
    float m_scale;             // gyroscope data scale
    int m_event_count; // sample data arrive
    bool m_calibrated;        // calibrate state
    gyro_cal_t m_cal_data;    // calibrate data
    filter_median_t m_filter; // filter data
};
}
