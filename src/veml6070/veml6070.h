/*
 * Author: Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "upm.h"
#include "mraa/i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

#define VEML6070_CTRL_REG             0x38    // write only
#define VEML6070_SEQ1_DATA_BUF_REG    0x39    // read only
#define VEML6070_SEQ2_DATA_BUF_REG    0x38    // read only

typedef enum {
    HALF_T = 0,
    ONE_T,
    TWO_T,
    FOUR_T } veml6070_integration_time_t;

/**
 * @file veml6070.h
 * @library veml6070
 * @brief C API for the VEML6070 Vishay UV Sensor
 *
 * @include veml6070.c
 */

typedef struct _veml6070_context {
    mraa_i2c_context       i2c_seq1;
    mraa_i2c_context       i2c_seq2;
    uint8_t                address_seq1;
    uint8_t                address_seq2;
    int                    i2c_bus_number;
} *veml6070_context;

/**
 * VEML6070 Initialization function
 *
 * @param bus I2C bus to use
 * @param address I2C address to use
 *
 * @return device context pointer
 */
veml6070_context veml6070_init(int bus);

/**
 * VEML6070 Close function
 *
 * @param dev veml6070_context pointer
 */
void veml6070_close(veml6070_context dev);

/**
 * Function to get the UV values
 *
 * @param dev veml6070_context pointer
 * @return int16_t UV value
 */
int16_t veml6070_get_uv_intensity(veml6070_context dev);

/**
 * Function to set the integration time of the sensor
 *
 * @param dev veml6070_context pointer
 * @param time veml6070_integration_time_t
 *
 * @return upm_result_t
 */
upm_result_t veml6070_set_integration_time(veml6070_context dev, veml6070_integration_time_t time);

#ifdef __cplusplus
}
#endif
