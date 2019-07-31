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

#include <iostream>
#include <string>
#include <stdexcept>
#include <string.h>
#include <math.h>
#include "mmc35240.hpp"

#define NUMBER_OF_BITS_IN_BYTE 8

/* Compass defines */
#define CONVERT_GAUSS_TO_MICROTESLA(x) ((x) *100)
#define EPSILON 0.000000001
#define MAGNETIC_LOW 960 /* 31 micro tesla squared */
#define CAL_STEPS 5

/* Filter defines */
#define FILTER_MAX_SAMPLE 20
#define FILTER_NUM_FILED 3

using namespace upm;
using namespace android;

/* We'll have multiple calibration levels so that we can provide an estimation as fast as possible
 */
static const float min_diffs[CAL_STEPS] = { 0.2, 0.25, 0.4, 0.6, 1.0 };
static const float max_sqr_errs[CAL_STEPS] = { 10.0, 10.0, 8.0, 5.0, 3.5 };
static const unsigned int lookback_counts[CAL_STEPS] = { 2, 3, 4, 5, 6 };

MMC35240::MMC35240(int device)
{
    float mag_scale;
    char trigger[64];

    if (!(m_iio = mraa_iio_init(device))) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_iio_init() failed, invalid device?");
        return;
    }
    m_scale = 1;
    m_iio_device_num = device;
    snprintf(trigger, 64, "hrtimer-mmc35240-hr-dev%d", device);

    if (mraa_iio_create_trigger(m_iio, trigger) != MRAA_SUCCESS)
        fprintf(stderr, "Create trigger %s failed\n", trigger);

    if (mraa_iio_get_mount_matrix(m_iio, "in_mount_matrix", m_mount_matrix) == MRAA_SUCCESS)
        m_mount_matrix_exist = true;
    else
        m_mount_matrix_exist = false;

    if (mraa_iio_read_float(m_iio, "in_magn_scale", &mag_scale) == MRAA_SUCCESS)
        m_scale = mag_scale;

    // calibration init data
    initCalibrate();

    // filter init data
    memset(&m_filter, 0, sizeof(filter_average_t));
    m_filter.max_samples = FILTER_MAX_SAMPLE;
    m_filter.num_fields = FILTER_NUM_FILED;
}

MMC35240::~MMC35240()
{
    if (m_filter.history) {
        free(m_filter.history);
        m_filter.history = NULL;
    }
    if (m_filter.history_sum) {
        free(m_filter.history_sum);
        m_filter.history_sum = NULL;
    }
    if (m_iio)
        mraa_iio_close(m_iio);
}

void
MMC35240::installISR(void (*isr)(char*, void*), void* arg)
{
    mraa_iio_trigger_buffer(m_iio, isr, arg);
}

int64_t
MMC35240::getChannelValue(unsigned char* input, mraa_iio_channel* chan)
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
MMC35240::enableBuffer(int length)
{
    mraa_iio_write_int(m_iio, "buffer/length", length);
    // enable must be last step, else will have error in writing above config
    mraa_iio_write_int(m_iio, "buffer/enable", 1);
    return true;
}

bool
MMC35240::disableBuffer()
{
    mraa_iio_write_int(m_iio, "buffer/enable", 0);
    return true;
}

bool
MMC35240::setScale(float scale)
{
    m_scale = scale;
    mraa_iio_write_float(m_iio, "in_magn_scale", scale);
    return true;
}

bool
MMC35240::setSamplingFrequency(float sampling_frequency)
{
    m_sampling_frequency = sampling_frequency;
    mraa_iio_write_float(m_iio, "in_magn_sampling_frequency", sampling_frequency);
    return true;
}

bool
MMC35240::enable3AxisChannel()
{
    char trigger[64];
    snprintf(trigger, 64, "mmc35240-hr-dev%d", m_iio_device_num);

    mraa_iio_write_string(m_iio, "trigger/current_trigger", trigger);
    mraa_iio_write_int(m_iio, "scan_elements/in_magn_x_en", 1);
    mraa_iio_write_int(m_iio, "scan_elements/in_magn_y_en", 1);
    mraa_iio_write_int(m_iio, "scan_elements/in_magn_z_en", 1);

    // need update channel data size after enable
    mraa_iio_update_channels(m_iio);
    return true;
}

void
MMC35240::extract3Axis(char* data, float* x, float* y, float* z)
{
    mraa_iio_channel* channels = mraa_iio_get_channels(m_iio);
    float tmp[3];
    int64_t iio_x, iio_y, iio_z;

    iio_x = getChannelValue((unsigned char*) (data), &channels[0]);
    iio_y = getChannelValue((unsigned char*) (data + 4), &channels[1]);
    iio_z = getChannelValue((unsigned char*) (data + 8), &channels[2]);

    // Raw data is magnetic field along axis x, y, and z. Units after application of scale are Gauss
    *x = CONVERT_GAUSS_TO_MICROTESLA(iio_x * m_scale);
    *y = CONVERT_GAUSS_TO_MICROTESLA(iio_y * m_scale);
    *z = CONVERT_GAUSS_TO_MICROTESLA(iio_z * m_scale);

    if (m_mount_matrix_exist) {
        tmp[0] = *x * m_mount_matrix[0] + *y * m_mount_matrix[1] + *z * m_mount_matrix[2];
        tmp[1] = *x * m_mount_matrix[3] + *y * m_mount_matrix[4] + *z * m_mount_matrix[5];
        tmp[2] = *x * m_mount_matrix[6] + *y * m_mount_matrix[7] + *z * m_mount_matrix[8];

        *x = tmp[0];
        *y = tmp[1];
        *z = tmp[2];
    }

    calibrateCompass(x, y, z, &m_cal_data);
    denoise_average(x, y, z);
}

int
MMC35240::getCalibratedLevel()
{
    return m_cal_level;
}

void
MMC35240::initCalibrate()
{
    m_cal_level = 0;
    resetSample(&m_cal_data);

    m_cal_data.offset[0][0] = 0;
    m_cal_data.offset[1][0] = 0;
    m_cal_data.offset[2][0] = 0;
    m_cal_data.w_invert[0][0] = 1;
    m_cal_data.w_invert[1][0] = 0;
    m_cal_data.w_invert[2][0] = 0;
    m_cal_data.w_invert[0][1] = 0;
    m_cal_data.w_invert[1][1] = 1;
    m_cal_data.w_invert[2][1] = 0;
    m_cal_data.w_invert[0][2] = 0;
    m_cal_data.w_invert[1][2] = 0;
    m_cal_data.w_invert[2][2] = 1;
    m_cal_data.bfield = 0;
}

void
MMC35240::getCalibratedData(int* cal_level, double offset[3][1], double w_invert[3][3], double* bfield)
{
    *cal_level = m_cal_level;
    offset[0][0] = m_cal_data.offset[0][0];
    offset[1][0] = m_cal_data.offset[1][0];
    offset[2][0] = m_cal_data.offset[2][0];
    w_invert[0][0] = m_cal_data.w_invert[0][0];
    w_invert[1][0] = m_cal_data.w_invert[1][0];
    w_invert[2][0] = m_cal_data.w_invert[2][0];
    w_invert[0][1] = m_cal_data.w_invert[0][1];
    w_invert[1][1] = m_cal_data.w_invert[1][1];
    w_invert[2][1] = m_cal_data.w_invert[2][1];
    w_invert[0][2] = m_cal_data.w_invert[0][2];
    w_invert[1][2] = m_cal_data.w_invert[1][2];
    w_invert[2][2] = m_cal_data.w_invert[2][2];
    *bfield = m_cal_data.bfield;
}

void
MMC35240::loadCalibratedData(int cal_level, double offset[3][1], double w_invert[3][3], double bfield)
{
    m_cal_level = cal_level;
    m_cal_data.offset[0][0] = offset[0][0];
    m_cal_data.offset[1][0] = offset[1][0];
    m_cal_data.offset[2][0] = offset[2][0];
    m_cal_data.w_invert[0][0] = w_invert[0][0];
    m_cal_data.w_invert[1][0] = w_invert[1][0];
    m_cal_data.w_invert[2][0] = w_invert[2][0];
    m_cal_data.w_invert[0][1] = w_invert[0][1];
    m_cal_data.w_invert[1][1] = w_invert[1][1];
    m_cal_data.w_invert[2][1] = w_invert[2][1];
    m_cal_data.w_invert[0][2] = w_invert[0][2];
    m_cal_data.w_invert[1][2] = w_invert[1][2];
    m_cal_data.w_invert[2][2] = w_invert[2][2];
    m_cal_data.bfield = bfield;
}

void
MMC35240::resetSample(compass_cal_t* data)
{
    int i, j;
    data->sample_count = 0;
    for (i = 0; i < MAGN_DS_SIZE; i++)
        for (j = 0; j < 3; j++)
            data->sample[i][j] = 0;

    data->average[0] = data->average[1] = data->average[2] = 0;
}

void
MMC35240::calibrateCompass(float* x, float* y, float* z, compass_cal_t* cal_data)
{
    int cal_level;

    /* Calibration is continuous */
    compassCollect(x, y, z, cal_data);

    cal_level = compassReady(cal_data);

    switch (cal_level) {
        case 0:
            scale(x, y, z);
            break;

        default:
            compassComputeCal(x, y, z, cal_data);
            break;
    }
}

int
MMC35240::compassCollect(float* x, float* y, float* z, compass_cal_t* cal_data)
{
    float data[3] = { *x, *y, *z };
    unsigned int index, j;
    unsigned int lookback_count;
    float min_diff;

    /* Discard the point if not valid */
    if (data[0] == 0 || data[1] == 0 || data[2] == 0)
        return -1;

    lookback_count = lookback_counts[m_cal_level];
    min_diff = min_diffs[m_cal_level];

    /* For the current point to be accepted, each x/y/z value must be different enough to the last
     * several collected points */
    if (cal_data->sample_count > 0 && cal_data->sample_count < MAGN_DS_SIZE) {
        unsigned int lookback =
        lookback_count < cal_data->sample_count ? lookback_count : cal_data->sample_count;
        for (index = 0; index < lookback; index++)
            for (j = 0; j < 3; j++)
                if (fabsf(data[j] - cal_data->sample[cal_data->sample_count - 1 - index][j]) <
                    min_diff) {
#ifdef DEBUG
                    printf("CompassCalibration:point reject: [%f,%f,%f], selected_count=%d\n",
                           data[0],
                           data[1],
                           data[2],
                           cal_data->sample_count);
#endif
                    return 0;
                }
    }

    if (cal_data->sample_count < MAGN_DS_SIZE) {
        memcpy(cal_data->sample[cal_data->sample_count], data, sizeof(float) * 3);
        cal_data->sample_count++;
        cal_data->average[0] += data[0];
        cal_data->average[1] += data[1];
        cal_data->average[2] += data[2];
#ifdef DEBUG
        printf("CompassCalibration:point collected [%f,%f,%f], selected_count=%d\n",
               (double) data[0],
               (double) data[1],
               (double) data[2],
               cal_data->sample_count);
#endif
    }
    return 1;
}

int
MMC35240::compassReady(compass_cal_t* cal_data)
{
    mat_input_t mat;
    int i;
    float max_sqr_err;

    compass_cal_t new_cal_data;

    /*
     * Some sensors take unrealistically long to calibrate at higher levels. We'll use a
     * max_cal_level if we have such a property setup,
     * or go with the default settings if not.
     */
    int cal_steps = CAL_STEPS;

    if (cal_data->sample_count < MAGN_DS_SIZE)
        return m_cal_level;

    max_sqr_err = max_sqr_errs[m_cal_level];

    /* Enough points have been collected, do the ellipsoid calibration */

    /* Compute average per axis */
    cal_data->average[0] /= MAGN_DS_SIZE;
    cal_data->average[1] /= MAGN_DS_SIZE;
    cal_data->average[2] /= MAGN_DS_SIZE;

    for (i = 0; i < MAGN_DS_SIZE; i++) {
        mat[i][0] = cal_data->sample[i][0];
        mat[i][1] = cal_data->sample[i][1];
        mat[i][2] = cal_data->sample[i][2];
    }

    /* Check if result is good. The sample data must remain the same */
    new_cal_data = *cal_data;

    if (ellipsoidFit(mat, new_cal_data.offset, new_cal_data.w_invert, &new_cal_data.bfield)) {
        double new_err = calcSquareErr(&new_cal_data);
#ifdef DEBUG
        printf("new err is %f, max sqr err id %f\n", new_err, max_sqr_err);
#endif
        if (new_err < max_sqr_err) {
            double err = calcSquareErr(cal_data);
            if (new_err < err) {
                /* New cal data is better, so we switch to the new */
                cal_data->offset = new_cal_data.offset;
                cal_data->w_invert = new_cal_data.w_invert;
                cal_data->bfield = new_cal_data.bfield;
                if (m_cal_level < (cal_steps - 1))
                    m_cal_level++;
#ifdef DEBUG
                printf("CompassCalibration: ready check success, caldata: %f %f %f %f %f %f %f %f "
                       "%f %f %f %f %f, err %f\n",
                       cal_data->offset[0][0],
                       cal_data->offset[1][0],
                       cal_data->offset[2][0],
                       cal_data->w_invert[0][0],
                       cal_data->w_invert[0][1],
                       cal_data->w_invert[0][2],
                       cal_data->w_invert[1][0],
                       cal_data->w_invert[1][1],
                       cal_data->w_invert[1][2],
                       cal_data->w_invert[2][0],
                       cal_data->w_invert[2][1],
                       cal_data->w_invert[2][2],
                       cal_data->bfield,
                       new_err);
#endif
            }
        }
    }
    resetSample(cal_data);
    return m_cal_level;
}

double
MMC35240::calcSquareErr(compass_cal_t* data)
{
    double err = 0;
    mat<double, 3, 1> raw, result, mat_diff;
    int i;
    float stdev[3] = { 0, 0, 0 };
    double diff;

    for (i = 0; i < MAGN_DS_SIZE; i++) {
        raw[0][0] = data->sample[i][0];
        raw[1][0] = data->sample[i][1];
        raw[2][0] = data->sample[i][2];


        stdev[0] += (raw[0][0] - data->average[0]) * (raw[0][0] - data->average[0]);
        stdev[1] += (raw[1][0] - data->average[1]) * (raw[1][0] - data->average[1]);
        stdev[2] += (raw[2][0] - data->average[2]) * (raw[2][0] - data->average[2]);

        mat_diff = raw - data->offset;
        result = multiply(data->w_invert, mat_diff);

        diff = sqrt(result[0][0] * result[0][0] + result[1][0] * result[1][0] +
                    result[2][0] * result[2][0]) -
               data->bfield;

        err += diff * diff;
    }

    stdev[0] = sqrt(stdev[0] / MAGN_DS_SIZE);
    stdev[1] = sqrt(stdev[1] / MAGN_DS_SIZE);
    stdev[2] = sqrt(stdev[2] / MAGN_DS_SIZE);

    /* A sanity check - if we have too little variation for an axis it's best to reject the
     * calibration than risking a wrong calibration */
    if (stdev[0] <= 1 || stdev[1] <= 1 || stdev[2] <= 1)
        return max_sqr_errs[0];

    err /= MAGN_DS_SIZE;
    return err;
}

int
MMC35240::ellipsoidFit(mat_input_t& m,
                       mat<double, 3, 1>& offset,
                       mat<double, 3, 3>& w_invert,
                       double* bfield)
{
    int i;
    mat<double, MAGN_DS_SIZE, 9> h;
    mat<double, MAGN_DS_SIZE, 1> w;
    mat<double, 9, MAGN_DS_SIZE> h_trans;
    mat<double, 9, 9> p_temp1;
    mat<double, 9, MAGN_DS_SIZE> p_temp2;
    mat<double, 3, 3> temp1, temp;
    mat<double, 3, 3> temp1_inv;
    mat<double, 3, 1> temp2;
    mat<double, 9, 9> result;
    mat<double, 9, 1> p;
    mat<double, 3, 3> a, sqrt_evals, evecs, evecs_trans;
    mat<double, 3, 1> evec1, evec2, evec3;

    for (i = 0; i < MAGN_DS_SIZE; i++) {
        w[i][0] = m[i][0] * m[i][0];
        h[i][0] = m[i][0];
        h[i][1] = m[i][1];
        h[i][2] = m[i][2];
        h[i][3] = -1 * m[i][0] * m[i][1];
        h[i][4] = -1 * m[i][0] * m[i][2];
        h[i][5] = -1 * m[i][1] * m[i][2];
        h[i][6] = -1 * m[i][1] * m[i][1];
        h[i][7] = -1 * m[i][2] * m[i][2];
        h[i][8] = 1;
    }

    h_trans = transpose(h);
    result = multiply(h_trans, h);
    p_temp1 = invert(result);
    p_temp2 = multiply(p_temp1, h_trans);
    p = multiply(p_temp2, w);

    temp1[0][0] = 2;
    temp1[0][1] = p[3][0];
    temp1[0][2] = p[4][0];
    temp1[1][0] = p[3][0];
    temp1[1][1] = 2 * p[6][0];
    temp1[1][2] = p[5][0];
    temp1[2][0] = p[4][0];
    temp1[2][1] = p[5][0];
    temp1[2][2] = 2 * p[7][0];

    temp2[0][0] = p[0][0];
    temp2[1][0] = p[1][0];
    temp2[2][0] = p[2][0];

    temp1_inv = invert(temp1);
    offset = multiply(temp1_inv, temp2);

    double off_x = offset[0][0];
    double off_y = offset[1][0];
    double off_z = offset[2][0];

    a[0][0] = 1.0 / (p[8][0] + off_x * off_x + p[6][0] * off_y * off_y + p[7][0] * off_z * off_z +
                     p[3][0] * off_x * off_y + p[4][0] * off_x * off_z + p[5][0] * off_y * off_z);

    a[0][1] = p[3][0] * a[0][0] / 2;
    a[0][2] = p[4][0] * a[0][0] / 2;
    a[1][2] = p[5][0] * a[0][0] / 2;
    a[1][1] = p[6][0] * a[0][0];
    a[2][2] = p[7][0] * a[0][0];
    a[2][1] = a[1][2];
    a[1][0] = a[0][1];
    a[2][0] = a[0][2];

    double eig1 = 0, eig2 = 0, eig3 = 0;
    computeEigenvalues(a, &eig1, &eig2, &eig3);

    if (eig1 <= 0 || eig2 <= 0 || eig3 <= 0)
        return 0;

    sqrt_evals[0][0] = sqrt(eig1);
    sqrt_evals[1][0] = 0;
    sqrt_evals[2][0] = 0;
    sqrt_evals[0][1] = 0;
    sqrt_evals[1][1] = sqrt(eig2);
    sqrt_evals[2][1] = 0;
    sqrt_evals[0][2] = 0;
    sqrt_evals[1][2] = 0;
    sqrt_evals[2][2] = sqrt(eig3);

    calcEvector(a, eig1, evec1);
    calcEvector(a, eig2, evec2);
    calcEvector(a, eig3, evec3);

    evecs[0][0] = evec1[0][0];
    evecs[1][0] = evec1[1][0];
    evecs[2][0] = evec1[2][0];
    evecs[0][1] = evec2[0][0];
    evecs[1][1] = evec2[1][0];
    evecs[2][1] = evec2[2][0];
    evecs[0][2] = evec3[0][0];
    evecs[1][2] = evec3[1][0];
    evecs[2][2] = evec3[2][0];

    temp1 = multiply(evecs, sqrt_evals);
    evecs_trans = transpose(evecs);
    temp = multiply(temp1, evecs_trans);
    w_invert = transpose(temp);

    *bfield = pow(sqrt(1 / eig1) * sqrt(1 / eig2) * sqrt(1 / eig3), 1.0 / 3.0);
    if (*bfield < 0)
        return 0;

    w_invert = w_invert * (*bfield);

    return 1;
}

/* Given an real symmetric 3x3 matrix A, compute the eigenvalues */
void
MMC35240::computeEigenvalues(mat<double, 3, 3>& A, double* eig1, double* eig2, double* eig3)
{
    double p = A[0][1] * A[0][1] + A[0][2] * A[0][2] + A[1][2] * A[1][2];

    if (p < EPSILON) {
        *eig1 = A[0][0];
        *eig2 = A[1][1];
        *eig3 = A[2][2];
        return;
    }

    double q = (A[0][0] + A[1][1] + A[2][2]) / 3;
    double temp1 = A[0][0] - q;
    double temp2 = A[1][1] - q;
    double temp3 = A[2][2] - q;

    p = temp1 * temp1 + temp2 * temp2 + temp3 * temp3 + 2 * p;
    p = sqrt(p / 6);

    mat<double, 3, 3> B = A;
    B[0][0] -= q;
    B[1][1] -= q;
    B[2][2] -= q;
    B = (1 / p) * B;

    double r =
    (B[0][0] * B[1][1] * B[2][2] + B[0][1] * B[1][2] * B[2][0] + B[0][2] * B[1][0] * B[2][1] -
     B[0][2] * B[1][1] * B[2][0] - B[0][0] * B[1][2] * B[2][1] - B[0][1] * B[1][0] * B[2][2]) /
    2;

    double phi;
    if (r <= -1.0)
        phi = M_PI / 3;
    else if (r >= 1.0)
        phi = 0;
    else
        phi = acos(r) / 3;

    *eig3 = q + 2 * p* cos(phi);
    *eig1 = q + 2 * p* cos(phi + 2 * M_PI / 3);
    *eig2 = 3 * q - *eig1 - *eig3;
}

void
MMC35240::calcEvector(mat<double, 3, 3>& A, double eig, mat<double, 3, 1>& vec)
{
    mat<double, 3, 3> h;
    mat<double, 2, 2> x_tmp;

    h = A;
    h[0][0] -= eig;
    h[1][1] -= eig;
    h[2][2] -= eig;

    mat<double, 2, 2> x;
    x[0][0] = h[1][1];
    x[0][1] = h[1][2];
    x[1][0] = h[2][1];
    x[1][1] = h[2][2];

    x_tmp = invert(x);
    x = x_tmp;

    double temp1 = x[0][0] * (-h[1][0]) + x[0][1] * (-h[2][0]);
    double temp2 = x[1][0] * (-h[1][0]) + x[1][1] * (-h[2][0]);
    double norm = sqrt(1 + temp1 * temp1 + temp2 * temp2);

    vec[0][0] = 1.0 / norm;
    vec[1][0] = temp1 / norm;
    vec[2][0] = temp2 / norm;
}

void
MMC35240::scale(float* x, float* y, float* z)
{
    float sqr_norm = 0;
    float sanity_norm = 0;
    float scale = 1;

    sqr_norm = (*x * *x + *y * *y + *z * *z);

    if (sqr_norm < MAGNETIC_LOW)
        sanity_norm = MAGNETIC_LOW;

    if (sanity_norm && sqr_norm) {
        scale = sanity_norm / sqr_norm;
        scale = sqrt(scale);
        *x = *x* scale;
        *y = *y* scale;
        *z = *z* scale;
    }
}

void
MMC35240::compassComputeCal(float* x, float* y, float* z, compass_cal_t* cal_data)
{
    mat<double, 3, 1> result, raw, diff;

    if (!m_cal_level)
        return;

    raw[0][0] = *x;
    raw[1][0] = *y;
    raw[2][0] = *z;

    diff = raw - cal_data->offset;
    result = multiply(cal_data->w_invert, diff);

    *x = result[0][0];
    *y = result[1][0];
    *z = result[2][0];

    scale(x, y, z);
}

void
MMC35240::denoise_average(float* x, float* y, float* z)
{
    /*
     * Smooth out incoming data using a moving average over a number of
     * samples. We accumulate one second worth of samples, or max_samples,
     * depending on which is lower.
     */
    float* data[3];
    int f;
    int history_size;
    int history_full = 0;
    filter_average_t* filter;

    data[0] = x;
    data[1] = y;
    data[2] = z;

    /* Don't denoise anything if we have less than two samples per second */
    if (m_sampling_frequency < 2)
        return;

    filter = (filter_average_t*) &m_filter;

    if (!filter)
        return;

    /* Restrict window size to the min of sampling_rate and max_samples */
    if (m_sampling_frequency > filter->max_samples)
        history_size = filter->max_samples;
    else
        history_size = m_sampling_frequency;

    /* Reset history if we're operating on an incorrect window size */
    if (filter->history_size != history_size) {
        filter->history_size = history_size;
        filter->history_entries = 0;
        filter->history_index = 0;
        filter->history =
        (float*) realloc(filter->history, filter->history_size * filter->num_fields * sizeof(float));
        if (filter->history) {
            filter->history_sum =
            (float*) realloc(filter->history_sum, filter->num_fields * sizeof(float));
            if (filter->history_sum)
                memset(filter->history_sum, 0, filter->num_fields * sizeof(float));
        }
    }

    if (!filter->history || !filter->history_sum)
        return; /* Unlikely, but still... */

    /* Update initialized samples count */
    if (filter->history_entries < filter->history_size)
        filter->history_entries++;
    else
        history_full = 1;

    /* Record new sample and calculate the moving sum */
    for (f = 0; f < filter->num_fields; f++) {
        /** A field is going to be overwritten if history is full, so decrease the history sum */
        if (history_full)
            filter->history_sum[f] -=
            filter->history[filter->history_index * filter->num_fields + f];

        filter->history[filter->history_index * filter->num_fields + f] = *data[f];
        filter->history_sum[f] += *data[f];

        /* For now simply compute a mobile mean for each field and output filtered data */
        *data[f] = filter->history_sum[f] / filter->history_entries;
    }

    /* Update our rolling index (next evicted cell) */
    filter->history_index = (filter->history_index + 1) % filter->history_size;
}
