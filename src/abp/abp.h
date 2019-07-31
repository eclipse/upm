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

#define ABP_DEFAULT_ADDRESS             0x28
#define OUTPUT_10_PERCENT_COUNT         1638
#define OUTPUT_90_PERCENT_COUNT         14746
#define ABP_PRESSURE_MAX                5
#define ABP_PRESSURE_MIN                0
#define ABP_PRESSURE_HIGH_BYTE_MASK     0x3f
#define ABP_TEMPERATURE_LOW_BYTE_SHIFT  5
#define ABP_TEMPERATURE_MASK            0x7ff

typedef enum {
    NORMAL_OPERATION = 0,
    COMMAND_MODE,
    STALE_DATA,
    DIAGNOSTIC } ABP_STATUS;

/**
 * @file abp.h
 * @library abp
 * @brief C API for the ABP Honeywell Pressure Sensor
 *
 * @include abp.c
 */
 
typedef struct _abp_context {
    mraa_i2c_context       i2c;
    int                    i2c_bus_number;
    uint8_t                address;
    int                    abp_pressure_max;
    int                    abp_pressure_min;
    uint8_t                readings[4];
    ABP_STATUS             status;
} *abp_context;

/**
 * ABP Initialization function
 *
 * @param bus I2C bus to use
 * @return device context pointer
 */
abp_context abp_init(int bus, uint8_t dev_address);

/**
 * ABP Close function
 *
 * @param dev abp_context pointer
 */
void abp_close(abp_context dev);

/**
 * Function to get the compensated pressure.
 * The datasheet provides a way to calculate the pressure.
 * In order to know the units take a look at the datasheet.
 *
 * @param dev abp_context pointer
 * @return float compensated pressure value
 */
float abp_get_pressure(abp_context dev);

/**
 * Function to retrieve the compensated temperature values.
 * All ABP seem to calculate temperature in degree Celsius.
 * (The above fact needs to be verified)
 * NOTE: This functionality is not available in all the ABP sensors.
 *
 * @param dev abp_context pointer
 * @return float compensated temperature value
 */
float abp_get_temperature(abp_context dev);

/**
 * Function to set the max pressure that the sensor
 * can read, can be retrieved from the datasheet, if this function is
 * not explicitly called to the set the max value, then a default max
 * value of 5 will be set.
 *
 * @param dev abp_context pointer
 * @param max int maximum pressure value possible
 */
void abp_set_max_pressure(abp_context dev, int max);

/**
 * Function to set the min pressure that the sensor
 * can read, can be retrieved from the datasheet, if this function is
 * not explicitly called to the set the min value, then a default min
 * value of 5 will be set.
 *
 * @param dev abp_context pointer
 * @param min int minimum pressure value possible
 */
void abp_set_min_pressure(abp_context dev, int min);

/**
 * This function has to be called before calling either of the get
 * temperature or get pressure functions. This function updates the
 * device context with the latest values. Not calling this function
 * will lead to stale values.
 *
 * @param dev abp_context pointer
 */
upm_result_t abp_update(abp_context dev);

#ifdef __cplusplus
}
#endif
