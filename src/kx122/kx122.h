/*
* The MIT License (MIT)
*
* Author: Samuli Rissanen <samuli.rissanen@hotmail.com>
* Copyright (c) 2018 Rohm Semiconductor.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/

#pragma once

#ifdef __cplusplus
extern "C"{
#endif

#include <assert.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>

#include <mraa/i2c.h>
#include <mraa/spi.h>
#include <mraa/gpio.h>

#include <upm.h>

#include "kx122_registers.h"

/**
 * @file kx122.h
 * @library kx122
 * @brief C API for the kx122 driver
 *
 * @include kx122.c
 */

//Frequency of the SPI connection
#define DEFAULT_SPI_FREQUENCY 10000

//Default slave addresses for the sensor
#define KX122_DEFAULT_SLAVE_ADDR_1 0x1F
#define KX122_DEFAULT_SLAVE_ADDR_2 0x1E

//Microseconds in a second
#define MICRO_S 1000000

//Sensor ODR values
typedef enum{
  KX122_ODR_12P5 = KX122_ODCNTL_OSA_12P5,
  KX122_ODR_25 = KX122_ODCNTL_OSA_25,
  KX122_ODR_50 = KX122_ODCNTL_OSA_50,
  KX122_ODR_100 = KX122_ODCNTL_OSA_100,
  KX122_ODR_200 = KX122_ODCNTL_OSA_200,
  KX122_ODR_400 = KX122_ODCNTL_OSA_400,
  KX122_ODR_800 = KX122_ODCNTL_OSA_800,
  KX122_ODR_1600 = KX122_ODCNTL_OSA_1600,
  KX122_ODR_0P781 = KX122_ODCNTL_OSA_0P781,
  KX122_ODR_1P563 = KX122_ODCNTL_OSA_1P563,
  KX122_ODR_3P125 = KX122_ODCNTL_OSA_3P125,
  KX122_ODR_6P25 = KX122_ODCNTL_OSA_6P25,
  KX122_ODR_3200 = KX122_ODCNTL_OSA_3200,
  KX122_ODR_6400 = KX122_ODCNTL_OSA_6400,
  KX122_ODR_12800 = KX122_ODCNTL_OSA_12800,
  KX122_ODR_25600 = KX122_ODCNTL_OSA_25600
} KX122_ODR_T;

//Sensor average values
typedef enum{
  KX122_NO_AVG = KX122_LP_CNTL_AVC_NO_AVG,
  KX122_2_SAMPLE_AVG = KX122_LP_CNTL_AVC_2_SAMPLE_AVG,
  KX122_4_SAMPLE_AVG = KX122_LP_CNTL_AVC_4_SAMPLE_AVG,
  KX122_8_SAMPLE_AVG = KX122_LP_CNTL_AVC_8_SAMPLE_AVG,
  KX122_16_SAMPLE_AVG = KX122_LP_CNTL_AVC_16_SAMPLE_AVG,
  KX122_32_SAMPLE_AVG = KX122_LP_CNTL_AVC_32_SAMPLE_AVG,
  KX122_64_SAMPLE_AVG = KX122_LP_CNTL_AVC_64_SAMPLE_AVG,
  KX122_128_SAMPLE_AVG = KX122_LP_CNTL_AVC_128_SAMPLE_AVG
} KX122_AVG_T;

//Sensor range values
typedef enum{
  KX122_RANGE_2G = KX122_CNTL1_GSEL_2G,
  KX122_RANGE_4G = KX122_CNTL1_GSEL_4G,
  KX122_RANGE_8G = KX122_CNTL1_GSEL_8G
} KX122_RANGE_T;

//Sensor buffer modes
typedef enum{
  KX122_FIFO_MODE = KX122_BUF_CNTL2_BUF_M_FIFO,
  KX122_FILO_MODE = KX122_BUF_CNTL2_BUF_M_FILO,
  KX122_STREAM_MODE = KX122_BUF_CNTL2_BUF_M_STREAM,
} KX122_BUFFER_MODE_T;

//Sensor interrupt types
typedef enum{
  KX122_BUF_FULL_INT = KX122_INC4_BFI1,
  KX122_WATERMARK_INT = KX122_INC4_WMI1,
  KX122_DATA_READY_INT = KX122_INC4_DRDYI1
} KX122_INTERRUPT_T;

//High and low resolution modes
typedef enum{
  HIGH_RES,
  LOW_RES
} KX122_RES_T;

//Sensor low-pass filter roll of values
typedef enum{
  ODR_9,
  ODR_2
} LPRO_STATE_T;

//Interrupt pins
typedef enum{
  INT1,
  INT2
}KX122_INTERRUPT_PIN_T;

//Interrupt polarity
typedef enum{
  ACTIVE_LOW,
  ACTIVE_HIGH
} KX122_INTERRUPT_POLARITY_T;

//Device context
typedef struct _kx122_context {
  mraa_i2c_context i2c;
  mraa_spi_context spi;
  mraa_gpio_context gpio1; //Interrupt pin 1
  mraa_gpio_context gpio2; //Interrupt pin 2
  mraa_gpio_context chip_select; //Chip select pin (SPI)

  float accel_scale; //Acceleration scaling
  KX122_RES_T res_mode; //Sensor resolution
  KX122_RANGE_T grange_mode; //Sensor range

  float buffer_accel_scale; //Buffer acceleration scaling
  KX122_BUFFER_MODE_T buffer_mode; //Buffer mode
  KX122_RES_T buffer_res; //Buffer resolution

  bool using_spi;

} *kx122_context;

//Struct for ODR values and their decimal counterparts.
typedef struct _odr_item{
  uint8_t odr_value;
  float odr_decimal;
} odr_item;

/**
KX122 initialization

Set addr to -1 if using SPI.
When using I2C, set chip_select_pin to -1;

If no errors occur, the device gets initialized with default values and gets set to active state.

@param bus I2C or SPI bus to use.
@param addr I2C address of the sensor.
@param chip_select Chip select pin for SPI.
@param spi_bus_frequency Speed of the SPI communication bus in Hz (ignored when using I2C).
@return The device context, or NULL if an error occurs.
*/
kx122_context kx122_init(int bus, int addr, int chip_select_pin, int spi_bus_frequency);

/**
KX122 destructor
Closes the I2C or SPI context, and removes interrupts.
Frees memory of the kx122_context.

@param dev The device context.
*/
void kx122_close(kx122_context dev);


/**
Intilializes the sensor with the given sampling rate, resolution and acceleration range.

This gets called during the kx122_init(), so it will not need to be called unless the device is reset.

Sensor is automatically set into standby mode during the initialization.
Sensor is set to active mode after initialization.

@param dev The device context.
@param odr One of the KX122_ODR_T values.
@param res One of the KX122_RES_T values.
@param grange One of the KX122_RANGE_T values.
@return UPM result.
*/
upm_result_t kx122_device_init(const kx122_context dev, KX122_ODR_T odr, KX122_RES_T res, KX122_RANGE_T grange);

/**
Gets the length of one sample period depending on the sampling rate of the sensor.

@param dev The device context.
@return Floating point value of the sampling period, or if an error occurs returns -1.
*/
float kx122_get_sample_period(const kx122_context dev);

/**
Gets who am i value of the sensor.

@param dev The device context.
@param data Pointer to a uint8_t variable to store the value.
@return UPM result.
*/
upm_result_t kx122_get_who_am_i(const kx122_context dev, uint8_t *data);

/**
Gets raw accelerometer data from the sensor.

@param dev The device context.
@param x Pointer to a floating point variable to store the x-axis value. Can be set to NULL if not wanted.
@param y Pointer to a floating point variable to store the y-axis value. Can be set to NULL if not wanted.
@param z Pointer to a floating point variable to store the z-axis value. Can be set to NULL if not wanted.
@return UPM result.
*/
upm_result_t kx122_get_acceleration_data_raw(const kx122_context dev, float *x, float *y, float *z);

/**
Gets converted (m/s^2) accelerometer data from the sensor.

@param dev The device context.
@param x Pointer to a floating point variable to store the x-axis value. Can be set to NULL if not wanted.
@param y Pointer to a floating point variable to store the y-axis value. Can be set to NULL if not wanted.
@param z Pointer to a floating point variable to store the z-axis value. Can be set to NULL if not wanted.
*/
upm_result_t kx122_get_acceleration_data(const kx122_context dev, float *x, float *y, float *z);

/**
Performs a sensor software reset. The software reset clears the RAM of the sensor and resets all registers
to pre-defined values.

You should call kx122_device_init() after the software reset.

See the datasheet for more details.

@param dev The device context.
@return UPM result.
*/
upm_result_t kx122_sensor_software_reset(const kx122_context dev);

/**
Enables sensor filter bypass.

Sensor needs to be in standby mode when enabling the filter bypass.

@param dev The device context.
@return UPM result.
*/
upm_result_t kx122_enable_iir(const kx122_context dev);

/**
Disables sensor filter bypass.

Sensor needs to be in standby mode when disabling the filter bypass.

@param dev The device context.
@return UPM result.
*/
upm_result_t kx122_disable_iir(const kx122_context dev);

/**
Performs a self-test of the sensor. The test applies an electrostatic force to the sensor,
simulating input acceleration. The test compares samples from all axis before and after
applying the electrostatic force to the sensor. If the amount of acceleration increases according
to the values defined in TABLE 1 of the datasheet, the test passes.

The function prints out the minimum, maximum and values calculated during the test
for each axis, and the result of the test for each axis.

See the datasheet for more information.

@param dev The device context.
@return UPM result.
*/
upm_result_t kx122_self_test(const kx122_context dev);

/**
Sets the sensor to the standby mode.

@param dev The device context.
@return UPM result.
*/
upm_result_t kx122_set_sensor_standby(const kx122_context dev);

/**
Sets the sensor to active mode.

@param dev The device context.
@return UPM result.
*/
upm_result_t kx122_set_sensor_active(const kx122_context dev);

/**
Sets the ODR of the sensor.

Sensor needs to be in standby mode when setting the ODR.

@param dev The device context.
@param odr One of the KX122_ODR_T values.
@return UPM result.
*/
upm_result_t kx122_set_odr(const kx122_context dev, KX122_ODR_T odr);

/**
Sets the acceleration range of the sensor.

Sensor needs to be in standby mode when setting the acceleration range value.

@param dev The device context.
@param grange One of the KX122_RANGE_T values.
@return UPM result.
*/
upm_result_t kx122_set_grange(const kx122_context dev, KX122_RANGE_T grange);

/**
Sets the resolution of the sensor. High resolution (16 bits) or low resolution (8 bits).

Sensor needs to be in standby mode when setting the sensor resolution.

When sensor is set to low resolution mode, the sensor runs in low power mode, which in turn
enables features such as averaging.(kx122_set_average()).

@param dev The device context.
@param res One of the KX122_RES_T values.
@return UPM result.
*/
upm_result_t kx122_set_resolution(const kx122_context dev, KX122_RES_T res);

/**
Sets the low pass filter roll off

Sensor needs to be in standby mode when setting the filter roll off value.

@param dev The device context.
@param lpro One of the LPRO_STATE_T values.
@return UPM result.
*/
upm_result_t kx122_set_bw(const kx122_context dev, LPRO_STATE_T lpro);

/**
Set the amount of samples to be averaged in low power mode.

Sensor needs to be in standby mode when setting the average value.

@param dev The device context.
@param avg One of the KX122_AVG_T values.
@return UPM result.
*/
upm_result_t kx122_set_average(const kx122_context dev, KX122_AVG_T avg);

/**
Installs an interrupt handler to be executed when an interrupt is detected on the interrupt pin.

@param dev The device context.
@param edge One of the mraa_gpio_edge_t values. Interrupt trigger edge.
@param intp One of the KX122_INTERRUPT_PIN_T values. Specifies which interrupt pin you are setting.
@param pin The GPIO pin to use as the interrupt pin.
@param isr Pointer to the function to be called, when the interrupt occurs.
@param arg The arguments to be passed to the function.
@return UPM result.
*/
upm_result_t kx122_install_isr(const kx122_context dev, mraa_gpio_edge_t edge,KX122_INTERRUPT_PIN_T intp, int pin,void (*isr)(void *), void *arg);

/**
Uninstalls a previously installed interrupt handler.

@param dev The device context.
@param intp One of the KX122_INTERRUPT_PIN_T values. Specifies which interrupt pin handler is uninstalled.
*/
void kx122_uninstall_isr(const kx122_context dev, KX122_INTERRUPT_PIN_T intp);

/**
Enables interrupts on the interrupt pin 1 of the sensor.

Pulse width = 50us (10us if data sampling rate > 1600Hz).
Using pulse mode.

Sensor needs to be in standby mode when enabling the interrupt.

See datasheet for more details.

@param dev The device context.
@param polarity One of the KX122_INTERRUPT_POLARITY_T values. Select the polarity of the interrupt.
@return UPM result.
*/
upm_result_t kx122_enable_interrupt1(const kx122_context dev, KX122_INTERRUPT_POLARITY_T polarity);

/**
Enables interrupts on the interrupt pin 2 of the sensor.

Pulse width = 50us (10us if data sampling rate > 1600Hz).
Using pulse mode.

Sensor needs to be in standby mode when enabling the interrupt.

See datasheet for more details.

@param dev The device context.
@param polarity One of the KX122_INTERRUPT_POLARITY_T values. Select the polarity of the interrupt.
@return UPM result.
*/
upm_result_t kx122_enable_interrupt2(const kx122_context dev, KX122_INTERRUPT_POLARITY_T polarity);

/**
Disables interrupts on the interrupt pin 1 of the sensor.

Sensor needs to be in standby mode when disabling the interrupt pin 1.

@param dev The device context.
@return UPM result.
*/
upm_result_t kx122_disable_interrupt1(const kx122_context dev);

/**
Disables interrupts on the interrupt pin 2 of the sensor.

Sensor needs to be in standby mode when disabling the interrupt pin 2.

@param dev The device context.
@return UPM result.
*/
upm_result_t kx122_disable_interrupt2(const kx122_context dev);

/**
Routes the interrupts to the interrupt pin 1 of the sensor.

Sensor needs to be in standby mode when routing the interrupts.

See datasheet for more details.

@param dev The device context.
@param bits One or more of the KX122_INTERRUPT_T values. Combine with bitwise OR (|)
@return UPM result.
*/
upm_result_t kx122_route_interrupt1(const kx122_context dev, uint8_t bits);

/**
Routes the interrupts to the interrupt pin 2 of the sensor.

Sensor needs to be in standby mode when routing the interrupts.

See datasheet for more details.

@param dev The device context.
@param bits One or more of the KX122_INTERRUPT_T values. Combine with bitwise OR (|)
@return UPM result.
*/
upm_result_t kx122_route_interrupt2(const kx122_context dev, uint8_t bits);

/**
Gets the status of the interrupts. (Has an interrupt occured)

See datasheet for more details.

@param dev The device context.
@return Return true if an interrupt event has occured, returns false if no interrupts have occured.
*/
bool kx122_get_interrupt_status(const kx122_context dev);

/**
Gets the source of the interrupt.
The value stored is one or more of the KX122_INTERRUPT_T values, depending on the interrupts
that have occured.

If multiple interrupts have occured, and you need to determine a specific interrupt,
you can use masking to get the state of the specific interrupt:
(int_source & KX122_DATA_READY_INT) == KX122_DATA_READY_INT)

See datasheet for more details.

@param dev The device context.
@param data Pointer to a uint8_t variable to store the value.
@return UPM result.
*/
upm_result_t kx122_get_interrupt_source(const kx122_context dev, uint8_t *data);

/**
Clears latching interrupts (Wakeup, Data Ready).

See datasheet for more details.

@param dev The device context.
@return UPM result.
*/
upm_result_t kx122_clear_interrupt(const kx122_context dev);

/**
Enables the data ready interrupt. Availability of new acceleration data is
reflected as an interrupt.

Sensor needs to be in standby mode when enabling the interrupt.

@param dev The device context.
@return UPM result.
*/
upm_result_t kx122_enable_data_ready_interrupt(const kx122_context dev);

/**
Disables the data ready interrupt.

Sensor needs to be in standby mode when disabling the interrupt.

@param dev The device context.
@return UPM result.
*/
upm_result_t kx122_disable_data_ready_interrupt(const kx122_context dev);

/**
Enables the buffer full interrupt.
Buffer can hold 681 sets of values (8 bit, low resolution mode) or
340 sets of values (16 bit, high resolution mode).

Sensor needs to be in standby mode when enabling the interrupt.

@param dev The device context.
@return UPM result.
*/
upm_result_t kx122_enable_buffer_full_interrupt(const kx122_context dev);

/**
Disables the buffer full interrupt.

Sensor needs to be in standby mode when disabling the interrupt.

@param dev The device context.
@return UPM result.
*/
upm_result_t kx122_disable_buffer_full_interrupt(const kx122_context dev);

/**
Enables the buffer.

Sensor needs to be in standby mode when enabling the buffer.

@param dev The device context.
@return UPM result.
*/
upm_result_t kx122_enable_buffer(const kx122_context dev);

/**
Disables the buffer.

Sensor needs to be in standby mode when disabling the buffer.

@param dev The device context.
@return UPM result.
*/
upm_result_t kx122_disable_buffer(const kx122_context dev);

/**
Initializes the buffer with the given sample watermark level, buffer resolution and buffer operating mode.
Buffer is enabled after the initialization.

Sensor is automatically set into standby mode during the buffer initialization.
Sensor is set to active mode after initialization.

See the other buffer functions for details about the different parameter values.

@param dev The device context.
@param samples Amount of samples to trigger the watermark interrupt.
@param res One of the KX122_RES_T values.
@param mode One of the KX122_BUFFER_MODE_T values.
@return UPM result.
*/
upm_result_t kx122_buffer_init(const kx122_context dev, uint samples, KX122_RES_T res, KX122_BUFFER_MODE_T mode);

/**
Sets the buffer resolution.
Buffer resolution is indepedent of the sensor resolution.

Sensor needs to be in standby mode when setting the buffer resolution.

@param dev The device context.
@return UPM result.
*/
upm_result_t kx122_set_buffer_resolution(const kx122_context dev, KX122_RES_T res);

/**
Sets the buffer watermark interrupt threshold.
When the buffer sample count reaches the watermark, watermark interrupt will be given.

In low resolution mode, maxiumum number of samples is 681. In high resolution, the maximum
number is 340.

See datasheet for more details.

Sensor needs to be in standby mode when setting the buffer threshold.

@param dev The device context.
@param samples Amount of samples to trigger the watermark interrupt.
@return UPM result.
*/
upm_result_t kx122_set_buffer_threshold(const kx122_context dev, uint samples);

/**
Sets the buffer operating mode.
The buffer can operate in FIFO,FILO or Stream mode.
The buffer gathers data, reports data and interracts with the status indicators
in a slightly different way depending on the operating mode.

See datasheet for more details.

Sensor needs to be in standby mode when setting the buffer mode.

@param dev The device context.
@param mode One of the KX122_BUFFER_MODE_T values.
@return UPM result.
*/
upm_result_t kx122_set_buffer_mode(const kx122_context dev, KX122_BUFFER_MODE_T mode);

/**
Gets the current amount of samples in the buffer.

@param dev The device context.
@param samples Pointer to an uint variable to store the data.
@return UPM result.
*/
upm_result_t kx122_get_buffer_status(const kx122_context dev, uint *samples);

/**
Gets the specified amount of raw acceleration samples from the buffer.

Make sure the array size is atleast the amount of samples to be read.

@param dev The device context.
@param len The amount of samples to read from the buffer.
@param x_array Pointer to an floating point array to store the x-axis data. Can be set to NULL if not wanted.
@param y_array Pointer to an floating point array to store the y-axis data. Can be set to NULL if not wanted.
@param z_array Pointer to an floating point array to store the z-axis data. Can be set to NULL if not wanted.
@return UPM result.
*/
upm_result_t kx122_read_buffer_samples_raw(const kx122_context dev, uint len, float *x_array, float *y_array, float *z_array);

/**
Gets the specified amount of converted (m/s^2) acceleration samples from the buffer.

Make sure the array size is atleast the amount of samples to be read.

@param dev The device context.
@param len The amount of samples to read from the buffer.
@param x_array Pointer to an floating point array to store the x-axis data. Can be set to NULL if not wanted.
@param y_array Pointer to an floating point array to store the y-axis data. Can be set to NULL if not wanted.
@param z_array Pointer to an floating point array to store the z-axis data. Can be set to NULL if not wanted.
@return UPM result.
*/
upm_result_t kx122_read_buffer_samples(const kx122_context dev, uint len, float *x_array, float *y_array, float *z_array);

/**
Clears the buffer, removing all existing samples from the buffer.

@param dev The device context.
@return UPM result.
*/
upm_result_t kx122_clear_buffer(const kx122_context dev);

#ifdef __cplusplus
}
#endif
