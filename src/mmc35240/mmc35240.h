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
 * Thanks to https://github.com/01org/android-iio-sensors-hal for magnetometer
 * calibration and denoise algorithm.
 */
#pragma once

#include <string>
#include <mraa/iio.h>

#define MAGN_DS_SIZE 32

namespace upm
{
/**
 * @brief MMC35240 Tri-axis Magnetic Sensor
 * @defgroup mmc35240 libupm-mmc35240
 * @ingroup STMicroelectronics iio i2c tri-axis magnetic sensor
 */

/**
 * @library mmc35240
 * @sensor mmc35240
 * @comname MMC35240 Tri-axis Magnetic Sensor
 * @type compass
 * @man Memsic
 * @con iio i2c
 *
 * @brief MMC35240 Tri-axis Magnetic Sensor API
 *
 * The MMC3524xPJ is a complete 3-axis magnetic sensor
 *
 * @snippet mmc35240.cxx Interesting
 */

class MMC35240
{
  public:
    typedef struct {
        /* hard iron offsets */
        double offset[3][1];

        /* soft iron matrix */
        double w_invert[3][3];

        /* geomagnetic strength */
        double bfield;

        /* selection data */
        float sample[MAGN_DS_SIZE][3];
        unsigned int sample_count;
        float average[3];
    } compass_cal_t;

    typedef double mat_input_t[MAGN_DS_SIZE][3];

    typedef struct {
        int max_samples;     /* Maximum averaging window size */
        int num_fields;      /* Number of fields per sample (usually 3) */
        float* history;      /* Working buffer containing recorded samples */
        float* history_sum;  /* The current sum of the history elements */
        int history_size;    /* Number of recorded samples */
        int history_entries; /* How many of these are initialized */
        int history_index;   /* Index of sample to evict next time */
    } filter_average_t;

    /**
     * MMC35240 Tri-axis Magnetic Sensor
     *
     * @param iio device number
     */
    MMC35240(int device);

    /**
     * MMC35240 destructor
     */
    ~MMC35240();

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
    void extract3Axis(char* data, float* x, float* y, float* z);

    /**
     * Get calibrated level
     */
    int getCalibratedLevel();

    /**
     * Reset calibration data and start collect calibration data again
     */
    void initCalibrate();

    /**
     * Get calibrated data
     */
    void getCalibratedData(double offset[3][1], double w_invert[3][3], double* bfield);

    /**
     * Load calibrated data
     */
    void
    loadCalibratedData(int cal_level, double offset[3][1], double w_invert[3][3], double bfield);

  private:
    /* Adopt https://github.com/01org/android-iio-sensors-hal/blob/master/matrix-ops.h */
    void transpose(int rows, int cols, void* m, void* m_trans);
    void multiply(int m, int n, int p, void* m1, void* m2, void* result);
    void invert(int s, void* m, void* m_inv);
    void multiply_scalar_inplace(int rows, int cols, void* m, double scalar);
    void assign(int rows, int cols, void* m, void* m1);
    void substract(int rows, int cols, void* m1, void* m2, void* res);

    /* Adopt https://github.com/01org/android-iio-sensors-hal/blob/master/compass-calibration.c */
    void resetSample(compass_cal_t* data);
    void calibrateCompass(float* x, float* y, float* z, compass_cal_t* cal_data);
    int compassCollect(float* x, float* y, float* z, compass_cal_t* cal_data);
    int compassReady(compass_cal_t* cal_data);
    double calcSquareErr(compass_cal_t* data);
    int ellipsoidFit(mat_input_t m, double offset[3][1], double w_invert[3][3], double* bfield);
    void computeEigenvalues(double mat[3][3], double* eig1, double* eig2, double* eig3);
    void calcEvector(double mat[3][3], double eig, double vec[3][1]);
    void scale(float* x, float* y, float* z);
    void compassComputeCal(float* x, float* y, float* z, compass_cal_t* cal_data);

    /* Adopt https://github.com/01org/android-iio-sensors-hal/blob/master/filtering.c */
    void denoise_average(float* x, float* y, float* z);

    mraa_iio_context m_iio;
    int m_iio_device_num;
    float m_sampling_frequency; // sampling frequency
    bool m_mount_matrix_exist;  // is mount matrix exist
    float m_mount_matrix[9];    // mount matrix
    float m_scale;              // data scale
    compass_cal_t m_cal_data;   // calibrate data
    int m_cal_level;            // calibrated level
    filter_average_t m_filter;  // filter data
};
}
