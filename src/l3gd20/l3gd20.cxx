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
 */

#include <iostream>
#include <string>
#include <stdexcept>
#include <string.h>
#include <math.h>
#include "l3gd20.hpp"

#define NUMBER_OF_BITS_IN_BYTE 8
#define GYRO_MIN_SAMPLES 5 /* Drop first few gyro samples after enable */
#define GYRO_MAX_ERR 0.05
#define GYRO_DS_SIZE 100

#define GYRO_DENOISE_MAX_SAMPLES 5
#define GYRO_DENOISE_NUM_FIELDS 3

using namespace upm;

L3GD20::L3GD20(int device)
{
    float gyro_scale;
    char trigger[64];

    if (!(m_iio = mraa_iio_init(device))) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_iio_init() failed, invalid device?");
        return;
    }
    m_scale = 1;
    m_iio_device_num = device;
    sprintf(trigger, "hrtimer-l3gd20-hr-dev%d", device);

    if (mraa_iio_create_trigger(m_iio, trigger) != MRAA_SUCCESS)
        fprintf(stderr, "Create trigger %s failed\n", trigger);

    if (mraa_iio_get_mounting_matrix(m_iio, m_mount_matrix) == MRAA_SUCCESS)
        m_mount_matrix_exist = true;
    else
        m_mount_matrix_exist = false;

    if (mraa_iio_read_float(m_iio, "in_anglvel_x_scale", &gyro_scale) == MRAA_SUCCESS)
        m_scale = gyro_scale;

    m_event_count = 0;

    // initial calibrate data
    initCalibrate();

    // initial denoise data
    m_filter.buff =
    (float*) calloc(GYRO_DENOISE_MAX_SAMPLES, sizeof(float) * GYRO_DENOISE_NUM_FIELDS);
    if (m_filter.buff == NULL) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_iio_init() failed, calloc denoise data");
        return;
    }
    m_filter.sample_size = GYRO_DENOISE_MAX_SAMPLES;
    m_filter.count = 0;
    m_filter.idx = 0;
}

L3GD20::~L3GD20()
{
    if (m_filter.buff) {
        free(m_filter.buff);
        m_filter.buff = NULL;
    }
    if(m_iio)
        mraa_iio_close(m_iio);
}

#ifdef JAVACALLBACK
void
L3GD20::installISR(IsrCallback* cb)
{
    installISR(generic_callback_isr, cb);
}
#endif

void
L3GD20::installISR(void (*isr)(char*), void* arg)
{
    mraa_iio_trigger_buffer(m_iio, isr, NULL);
}

int64_t
L3GD20::getChannelValue(unsigned char* input, mraa_iio_channel* chan)
{
    uint64_t u64 = 0;
    int i;
    int storagebits = chan->bytes * NUMBER_OF_BITS_IN_BYTE;
    int realbits = chan->bits_used;
    int zeroed_bits = storagebits - realbits;
    uint64_t sign_mask;
    uint64_t value_mask;

    if (!chan->lendian)
        for (i = 0; i < storagebits / NUMBER_OF_BITS_IN_BYTE; i++)
            u64 = (u64 << 8) | input[i];
    else
        for (i = storagebits / NUMBER_OF_BITS_IN_BYTE - 1; i >= 0; i--)
            u64 = (u64 << 8) | input[i];

    u64 = (u64 >> chan->shift) & (~0ULL >> zeroed_bits);

    if (!chan->signedd)
        return (int64_t) u64; /* We don't handle unsigned 64 bits int */

    /* Signed integer */

    switch (realbits) {
        case 0 ... 1:
            return 0;

        case 8:
            return (int64_t)(int8_t) u64;

        case 16:
            return (int64_t)(int16_t) u64;

        case 32:
            return (int64_t)(int32_t) u64;

        case 64:
            return (int64_t) u64;

        default:
            sign_mask = 1 << (realbits - 1);
            value_mask = sign_mask - 1;

            if (u64 & sign_mask)
                return -((~u64 & value_mask) + 1); /* Negative value: return 2-complement */
            else
                return (int64_t) u64; /* Positive value */
    }
}

bool
L3GD20::enableBuffer(int length)
{
    mraa_iio_write_int(m_iio, "buffer/length", length);
    // enable must be last step, else will have error in writing above config
    mraa_iio_write_int(m_iio, "buffer/enable", 1);
    return true;
}

bool
L3GD20::disableBuffer()
{
    mraa_iio_write_int(m_iio, "buffer/enable", 0);
    return true;
}

bool
L3GD20::setScale(float scale)
{
    mraa_iio_write_float(m_iio, "in_anglvel_x_scale", scale);
    mraa_iio_write_float(m_iio, "in_anglvel_y_scale", scale);
    mraa_iio_write_float(m_iio, "in_anglvel_z_scale", scale);
    return true;
}

bool
L3GD20::setSamplingFrequency(float sampling_frequency)
{
    mraa_iio_write_float(m_iio, "sampling_frequency", sampling_frequency);
    return true;
}

bool
L3GD20::enable3AxisChannel()
{
    char trigger[64];
    sprintf(trigger, "l3gd20-hr-dev%d", m_iio_device_num);

    mraa_iio_write_string(m_iio, "trigger/current_trigger", trigger);
    mraa_iio_write_int(m_iio, "scan_elements/in_anglvel_x_en", 1);
    mraa_iio_write_int(m_iio, "scan_elements/in_anglvel_y_en", 1);
    mraa_iio_write_int(m_iio, "scan_elements/in_anglvel_z_en", 1);

    // need update channel data size after enable
    mraa_iio_update_channels(m_iio);
    return true;
}

bool
L3GD20::extract3Axis(char* data, float* x, float* y, float* z)
{
    mraa_iio_channel* channels = mraa_iio_get_channels(m_iio);
    float tmp[3];
    int iio_x, iio_y, iio_z;

    m_event_count++;

    if (m_event_count < GYRO_MIN_SAMPLES) {
        /* drop the sample */
        return false;
    }

    iio_x = getChannelValue((unsigned char*) (data + channels[0].location), &channels[0]);
    iio_y = getChannelValue((unsigned char*) (data + channels[1].location), &channels[1]);
    iio_z = getChannelValue((unsigned char*) (data + channels[2].location), &channels[2]);

    *x = (iio_x * m_scale);
    *y = (iio_y * m_scale);
    *z = (iio_z * m_scale);

    if (m_mount_matrix_exist) {
        tmp[0] = *x * m_mount_matrix[0] + *y * m_mount_matrix[1] + *z * m_mount_matrix[2];
        tmp[1] = *x * m_mount_matrix[3] + *y * m_mount_matrix[4] + *z * m_mount_matrix[5];
        tmp[2] = *x * m_mount_matrix[6] + *y * m_mount_matrix[7] + *z * m_mount_matrix[8];

        *x = tmp[0];
        *y = tmp[1];
        *z = tmp[2];
    }

    /* Attempt gyroscope calibration if we have not reached this state */
    if (m_calibrated == false)
        m_calibrated = gyroCollect(*x, *y, *z);

    *x = *x - m_cal_data.bias_x;
    *y = *y - m_cal_data.bias_y;
    *z = *z - m_cal_data.bias_z;


    gyroDenoiseMedian(x, y, z);
    clampGyroReadingsToZero(x, y, z);

    return true;
}

void
L3GD20::initCalibrate()
{
    m_calibrated = false;
    m_cal_data.count = 0;
    m_cal_data.bias_x = m_cal_data.bias_y = m_cal_data.bias_z = 0;
    m_cal_data.min_x = m_cal_data.min_y = m_cal_data.min_z = 1.0;
    m_cal_data.max_x = m_cal_data.max_y = m_cal_data.max_z = -1.0;
}

bool
L3GD20::getCalibratedStatus()
{
    return m_calibrated;
}

void
L3GD20::getCalibratedData(float* bias_x, float* bias_y, float* bias_z)
{
    *bias_x = m_cal_data.bias_x;
    *bias_y = m_cal_data.bias_y;
    *bias_z = m_cal_data.bias_z;
}

void
L3GD20::loadCalibratedData(float bias_x, float bias_y, float bias_z)
{
    m_calibrated = true;
    m_cal_data.bias_x = bias_x;
    m_cal_data.bias_y = bias_y;
    m_cal_data.bias_z = bias_z;
}

bool
L3GD20::gyroCollect(float x, float y, float z)
{
    /* Analyze gyroscope data */

    if (fabs(x) >= 1 || fabs(y) >= 1 || fabs(z) >= 1) {
        /* We're supposed to be standing still ; start over */
        m_cal_data.count = 0;
        m_cal_data.bias_x = m_cal_data.bias_y = m_cal_data.bias_z = 0;
        m_cal_data.min_x = m_cal_data.min_y = m_cal_data.min_z = 1.0;
        m_cal_data.max_x = m_cal_data.max_y = m_cal_data.max_z = -1.0;

        return false; /* Uncalibrated */
    }

    /* Thanks to https://github.com/01org/android-iio-sensors-hal for calibration algorithm */
    if (m_cal_data.count < GYRO_DS_SIZE) {
        if (x < m_cal_data.min_x)
            m_cal_data.min_x = x;

        if (y < m_cal_data.min_y)
            m_cal_data.min_y = y;

        if (z < m_cal_data.min_z)
            m_cal_data.min_z = z;

        if (x > m_cal_data.max_x)
            m_cal_data.max_x = x;

        if (y > m_cal_data.max_y)
            m_cal_data.max_y = y;

        if (z > m_cal_data.max_z)
            m_cal_data.max_z = z;

        if (fabs(m_cal_data.max_x - m_cal_data.min_x) <= GYRO_MAX_ERR &&
            fabs(m_cal_data.max_y - m_cal_data.min_y) <= GYRO_MAX_ERR &&
            fabs(m_cal_data.max_z - m_cal_data.min_z) <= GYRO_MAX_ERR)
            m_cal_data.count++; /* One more conformant sample */
        else {
            /* Out of spec sample ; start over */
            m_calibrated = false;
            m_cal_data.count = 0;
            m_cal_data.bias_x = m_cal_data.bias_y = m_cal_data.bias_z = 0;
            m_cal_data.min_x = m_cal_data.min_y = m_cal_data.min_z = 1.0;
            m_cal_data.max_x = m_cal_data.max_y = m_cal_data.max_z = -1.0;
        }

        return false; /* Still uncalibrated */
    }

    /* We got enough stable samples to estimate gyroscope bias */
    m_cal_data.bias_x = (m_cal_data.max_x + m_cal_data.min_x) / 2;
    m_cal_data.bias_y = (m_cal_data.max_y + m_cal_data.min_y) / 2;
    m_cal_data.bias_z = (m_cal_data.max_z + m_cal_data.min_z) / 2;

    return true; /* Calibrated! */
}

void
L3GD20::gyroDenoiseMedian(float* x, float* y, float* z)
{
    /* Thanks to https://github.com/01org/android-iio-sensors-hal for denoise algorithm */
    unsigned int offset;

    /* If we are at event count 1 reset the indices */
    if (m_event_count == 1) {
        m_filter.count = 0;
        m_filter.idx = 0;
    }

    if (m_filter.count < m_filter.sample_size)
        m_filter.count++;

    offset = 0;
    m_filter.buff[offset + m_filter.idx] = *x;
    *x = median(m_filter.buff + offset, m_filter.count);

    offset = m_filter.sample_size * 1;
    m_filter.buff[offset + m_filter.idx] = *y;
    *y = median(m_filter.buff + offset, m_filter.count);

    offset = m_filter.sample_size * 2;
    m_filter.buff[offset + m_filter.idx] = *z;
    *z = median(m_filter.buff + offset, m_filter.count);

    m_filter.idx = (m_filter.idx + 1) % m_filter.sample_size;
}

float
L3GD20::median(float* queue, unsigned int size)
{
    /* http://en.wikipedia.org/wiki/Quickselect */

    unsigned int left = 0;
    unsigned int right = size - 1;
    unsigned int pivot_index;
    unsigned int median_index = (right / 2);
    float temp[size];

    memcpy(temp, queue, size * sizeof(float));

    /* If the list has only one element return it */
    if (left == right)
        return temp[left];

    while (left < right) {
        pivot_index = (left + right) / 2;
        pivot_index = partition(temp, left, right, pivot_index);
        if (pivot_index == median_index)
            return temp[median_index];
        else if (pivot_index > median_index)
            right = pivot_index - 1;
        else
            left = pivot_index + 1;
    }

    return temp[left];
}

unsigned int
L3GD20::partition(float* list, unsigned int left, unsigned int right, unsigned int pivot_index)
{
    unsigned int i;
    unsigned int store_index = left;
    float aux;
    float pivot_value = list[pivot_index];

    /* Swap list[pivotIndex] and list[right] */
    aux = list[pivot_index];
    list[pivot_index] = list[right];
    list[right] = aux;

    for (i = left; i < right; i++) {
        if (list[i] < pivot_value) {
            /* Swap list[store_index] and list[i] */
            aux = list[store_index];
            list[store_index] = list[i];
            list[i] = aux;
            store_index++;
        }
    }

    /* Swap list[right] and list[store_index] */
    aux = list[right];
    list[right] = list[store_index];
    list[store_index] = aux;
    return store_index;
}

void
L3GD20::clampGyroReadingsToZero(float* x, float* y, float* z)
{
    float near_zero;

    /* If we're calibrated, don't filter out as much */
    if (m_calibrated)
        near_zero = 0.02; /* rad/s */
    else
        near_zero = 0.1;

    /* If motion on all axes is small enough */
    if (fabs(*x) < near_zero && fabs(*y) < near_zero && fabs(*z) < near_zero) {
        /*
         * Report that we're not moving at all... but not exactly zero as composite sensors
         * (orientation, rotation vector) don't
         * seem to react very well to it.
         */

        *x *= 0.000001;
        *y *= 0.000001;
        *z *= 0.000001;
    }
}
