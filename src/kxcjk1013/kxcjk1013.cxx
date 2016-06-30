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

#include <iostream>
#include <string>
#include <stdexcept>
#include <string.h>
#include "kxcjk1013.hpp"

#define NUMBER_OF_BITS_IN_BYTE 8

using namespace upm;

KXCJK1013::KXCJK1013(int device)
{
    float accel_scale;
    char trigger[64];

    if (!(m_iio = mraa_iio_init(device))) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_iio_init() failed, invalid device?");
        return;
    }
    m_scale = 1;
    m_iio_device_num = device;
    sprintf(trigger, "hrtimer-kxcjk1013-hr-dev%d", device);

    if (mraa_iio_create_trigger(m_iio, trigger) != MRAA_SUCCESS)
        fprintf(stderr, "Create trigger %s failed\n", trigger);

    if (mraa_iio_get_mount_matrix(m_iio, "in_mount_matrix", m_mount_matrix) == MRAA_SUCCESS)
        m_mount_matrix_exist = true;
    else
        m_mount_matrix_exist = false;

    if (mraa_iio_read_float(m_iio, "in_accel_scale", &accel_scale) == MRAA_SUCCESS)
        m_scale = accel_scale;
}

KXCJK1013::~KXCJK1013()
{
    if (m_iio)
        mraa_iio_close(m_iio);
}

void
KXCJK1013::installISR(void (*isr)(char*), void* arg)
{
    mraa_iio_trigger_buffer(m_iio, isr, NULL);
}

int64_t
KXCJK1013::getChannelValue(unsigned char* input, mraa_iio_channel* chan)
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
            u64 = (u64 << NUMBER_OF_BITS_IN_BYTE) | input[i];
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
KXCJK1013::enableBuffer(int length)
{
    mraa_iio_write_int(m_iio, "buffer/length", length);
    // enable must be last step, else will have error in writing above config
    mraa_iio_write_int(m_iio, "buffer/enable", 1);

    return true;
}

bool
KXCJK1013::disableBuffer()
{
    mraa_iio_write_int(m_iio, "buffer/enable", 0);

    return true;
}

bool
KXCJK1013::setScale(float scale)
{
    m_scale = scale;
    mraa_iio_write_float(m_iio, "in_accel_scale", scale);

    return true;
}

bool
KXCJK1013::setSamplingFrequency(float sampling_frequency)
{
    mraa_iio_write_float(m_iio, "in_accel_sampling_frequency", sampling_frequency);

    return true;
}

bool
KXCJK1013::enable3AxisChannel()
{
    char trigger[64];
    sprintf(trigger, "kxcjk1013-hr-dev%d", m_iio_device_num);

    mraa_iio_write_string(m_iio, "trigger/current_trigger", trigger);
    mraa_iio_write_int(m_iio, "scan_elements/in_accel_x_en", 1);
    mraa_iio_write_int(m_iio, "scan_elements/in_accel_y_en", 1);
    mraa_iio_write_int(m_iio, "scan_elements/in_accel_z_en", 1);

    // need update channel data size after enable
    mraa_iio_update_channels(m_iio);
    return true;
}

void
KXCJK1013::extract3Axis(char* data, float* x, float* y, float* z)
{
    mraa_iio_channel* channels = mraa_iio_get_channels(m_iio);
    float tmp[3];
    int i = 0;
    int iio_x, iio_y, iio_z;

    iio_x = getChannelValue((unsigned char*) (data + channels[0].location), &channels[0]);
    iio_y = getChannelValue((unsigned char*) (data + channels[1].location), &channels[1]);
    iio_z = getChannelValue((unsigned char*) (data + channels[2].location), &channels[2]);

    // Raw data is acceleration in direction. Units after application of scale are m/s^2
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
}
