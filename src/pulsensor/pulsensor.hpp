/*
 * Author: Andrei Vasiliu <andrei.vasiliu@intel.com>
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Credits to Adafruit.
 * Based on Adafruit BMP085 library.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>
#include <math.h>
#include <mraa/pwm.hpp>
#include <mraa/aio.hpp>
#include <mraa/gpio.hpp>
#include <pthread.h>

#define HIGH               1
#define LOW                0

#define TRUE               HIGH
#define FALSE              LOW

/**
 * Callback data struct
 */
struct clbk_data {
    int is_heart_beat; /**< heartbeat check */
};

#include "Callback.hpp"

namespace upm {
/**
 * @brief Pulsensor Pulse Sensor
 * @defgroup pulsensor libupm-pulsensor
 * @ingroup seeed analog medical
 */
/**
 * @library pulsensor
 * @sensor pulsensor
 * @comname 3-wire Pulse Sensor
 * @type medical
 * @man seeed
 * @web http://www.adafruit.com/products/1093
 * @con analog
 *
 * @brief C++ API for the 3-Wire Pulse Sensor
 *
 * This is a library for a 3-wire pulse sensor sold by several manufacturers.
 * Usually, you can identify the sensor based on the round breakout and the
 * distinctive heart symbol.
 *
 * @image html pulsensor.jpg
 * @snippet pulsensor.cxx Interesting
 */
class Pulsensor {

public:
    Pulsensor(Callback *callback);

    void start_sampler();
    void stop_sampler();

private:
    static void      *do_sample(void *arg);
    pthread_t        sample_thread; /**< Thread for the code sample */
    uint32_t         sample_counter; /**< Counter for the code sample */
    uint32_t         last_beat_time; /**< Last heartbeat time */
    int              threshold; /**< Threshold */
    int              ibi_rate[10]; /**< ibi rate */
    int              ibi; /**< ibi */
    int              trough; /**< Trough */
    int              peak; /**< Peak */
    int              bpm; /**< Bits per minute */
    int              apmlitude; /**< Amplitude */
    uint8_t          qs; /**< qs */
    uint8_t          is_pulse; /**< Is pulse check */
    uint8_t          first_beat; /**< First heartbeat */
    uint8_t          second_beat; /**< Second heartbeat */
    uint8_t          ret; /**< Return value */
    mraa::Aio        pin_ctx; /**< The pin context */

    Callback *obj_callback; /**< The callback object */
    volatile uint16_t ctx_counter;
};
}
