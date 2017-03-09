/*
 * Author: Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2017 Intel Corporation.
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

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "upm.h"
#include "mraa/i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ABPDRRT005PG2A5_ADDRESS       0x28
#define OUTPUT_10_PERCENT_COUNT       1638
#define OUTPUT_90_PERCENT_COUNT       14746
#define ABPDRRT005PG2A5_PRESSURE_MAX  5
#define ABPDRRT005PG2A5_PRESSURE_MIN  0

/**
 * @file abpdrrt005pg2a5.h
 * @library abpdrrt005pg2a5
 * @brief C API for the ABPDRRT005PG2A5 Honeywell Pressure Sensor
 *
 * @include abpdrrt005pg2a5.c
 */
 
 typedef struct _abpdrrt005pg2a5_context {
    mraa_i2c_context       i2c;
    uint8_t                i2c_bus_number;
	uint8_t                address;
} *abpdrrt005pg2a5_context;

/**
 * ABPDRRT005PG2A5 Initialization function
 *
 * @param bus I2C bus to use
 * @return device context pointer
 */
abpdrrt005pg2a5_context abpdrrt005pg2a5_init(uint8_t bus, uint8_t dev_address);

/**
 * ABPDRRT005PG2A5 Close function
 *
 * @param dev abpdrrt005pg2a5_context pointer
 */
void abpdrrt005pg2a5_close(abpdrrt005pg2a5_context dev);

/**
 * Function to get the pressure in psi
 * The datasheet provides a way to calculate the pressure
 * in psi.
 *
 * @return upm_result_t UPM success/error code
 */
upm_result_t abpdrrt005pg2a5_get_pressure_psi(abpdrrt005pg2a5_context dev, float* pressure_psi);

/**
 * Function to get the pressure in pascal
 * This is calculated using a conversion function.
 *
 * @return upm_result_t UPM success/error code
 */
upm_result_t abpdrrt005pg2a5_get_pressure_pascal(abpdrrt005pg2a5_context dev, float* pressure_pascal);

#ifdef __cplusplus
}
#endif