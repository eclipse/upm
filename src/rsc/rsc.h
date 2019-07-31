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
#include "mraa/spi.h"
#include "mraa/gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "rsc_regs.h"
/**
 * @file rsc.h
 * @library rsc
 * @brief C API for the rsc driver
 *
 * @include rsc.c
 */

/**
 * Device context
 */
typedef struct _rsc_context {
    mraa_spi_context       spi;
    mraa_gpio_context      cs_ee;
    mraa_gpio_context      cs_adc;
    int                    spi_bus_number;
    float                  coeff_matrix[RSC_COEFF_T_ROW_NO][RSC_COEFF_T_COL_NO];
    PRESSURE_U             unit;
    PRESSURE_T             type;
    float                  pressure_range;
    float                  min_pressure_val;
    RSC_DATA_RATE          data_rate;
    RSC_MODE               mode;
    uint16_t               t_raw;
} *rsc_context;

/**
 * RSC initialization.
 *
 * This device supports SPI. The sensor has two chip select
 * pins since there are two ways to access content on the
 * sensor - EEPROM/ADC. We will not be using the SPI CS here
 * because that would work for all SPI operations. Hence, you are
 * expected to initialize any 2 gpio pins for CS by providing the
 * pin numbers.
 *
 * @param bus SPI bus to use.
 * @param cs_ee_pin The CS pin for accessing the EEPROM
 * @param cs_adc_pin The CS pin for accessing the ADC
 * @return The device context, or NULL if an error occurred.
 */
rsc_context rsc_init(int bus, int cs_ee_pin, int cs_adc_pin);

/**
 * RSC Close function
 *
 * @param dev The device context
 */
upm_result_t rsc_close(rsc_context dev);

/**
 * This function fills up a character array provided to it with
 * the sensor name that it reads from the EEPROM.
 * NOTE: The length of the array will always have to be RSC_SENSOR_NAME_LEN
 *
 * @param dev The device context
 * @param sensor_name Pointer to a uint8_t array with length
 * RSC_SENSOR_NAME_LEN. This array will be filled up with the sensor name
 * read from the EEPROM.
 * @return UPM result.
 */
upm_result_t rsc_get_sensor_name(rsc_context dev, uint8_t* sensor_name);

/**
 * This function fills up a character array provided to it with
 * the sensor serial number that it reads from the EEPROM.
 * NOTE: The length of the array will always have to be RSC_SENSOR_NUMBER_LEN
 *
 * @param dev The device context
 * @param sensor_name Pointer to a uint8_t array with length
 * RSC_SENSOR_NUMBER_LEN. This array will be filled up with the serial number
 * @return UPM result.
 */
upm_result_t rsc_get_sensor_serial_number(rsc_context dev, uint8_t* rsc_number);

/**
 * This function reads the EEPROM to retrieve the pressure range for the
 * particular RSC device.
 *
 * @param dev The device context
 * @return Sensor pressure range in float
 */
float rsc_get_pressure_range(rsc_context dev);

/**
 * This function reads the EEPROM to retrieve the minimum pressure that
 * the sensor can read.
 *
 * @param dev The device context
 * @return Minimum pressure sensor can read in float
 */
float rsc_get_minimum_pressure(rsc_context dev);

/**
 * This function reads the EEPROM to retrieve the pressure units that
 * the pressure is read in
 *
 * @param dev The device context
 * @return enum PRESSURE_U
 */
PRESSURE_U rsc_get_pressure_unit(rsc_context dev);

/**
 * This function reads the EEPROM to retrieve the sensor pressure type
 *
 * @param dev The device context
 * @return enum PRESSURE_T
 */
PRESSURE_T rsc_get_pressure_type(rsc_context dev);

/**
 * This function reads the EEPROM to extract the values with which the
 * ADC has to be initialized.
 * NOTE: The length of the array will always be 4
 *
 * @param dev The device context
 * @param adc_init_values uint8_t pointer to an array containing the
 * initial values for the ADC
 * @return UPM result.
 */
upm_result_t rsc_get_initial_adc_values(rsc_context dev, uint8_t* adc_init_values);

/**
 * This function is used to retrieve the coefficients from the EEPROM.
 * Once retrieved, the function updates a 2 dimensional array in the
 * sensor context with the latest values. This function doesn't return
 * the coefficient values.
 *
 * @param dev The device context
 * @return UPM result.
 */
upm_result_t rsc_retrieve_coefficients(rsc_context dev);

/**
 * This function sets up the initial values in the ADC and also
 * sets is to a default data rate - normal with 20 samples per second
 *
 * @param dev The device context
 * @return UPM result.
 */
upm_result_t rsc_setup_adc(rsc_context dev, uint8_t* adc_init_values);

/**
 * Function to get the compensated temperature as read by the sensor.
 *
 * @param dev The device context
 * @return float temperature in degree Celsius
 */
float rsc_get_temperature(rsc_context dev);

/**
 * Function to get the compensated pressure as read by the sensor.
 *
 * @param dev The device context
 * @return float pressure (units inH2O)
 */
float rsc_get_pressure(rsc_context dev);

/**
 * Function to set the mode of the sensor.
 *
 * @param dev The device context
 * @param mode mode of the sensor
 * @return UPM result.
 */
upm_result_t rsc_set_mode(rsc_context dev, RSC_MODE mode);

/**
 * Function to set the data rate of the sensor.
 *
 * @param dev The device context
 * @param dr data rate of the sensor
 * @return UPM result.
 */
upm_result_t rsc_set_data_rate(rsc_context dev, RSC_DATA_RATE dr);

#ifdef __cplusplus
}
#endif
