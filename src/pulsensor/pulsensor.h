/*
 * Author: Andrei Vasiliu <andrei.vasiliu@intel.com>
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Credits to Adafruit.
 * Based on Adafruit BMP085 library.
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
 * @brief Pulsensor Pulse Sensor library
 * @defgroup pulsensor libupm-pulsensor
 * @ingroup seeed analog medical
 */
/**
 * @library pulsensor
 * @sensor pulsensor
 * @comname Pulse Sensor
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

/*!
 * @struct clbk_data
 * @brief callback data 
 */
struct clbk_data {
    int is_heart_beat; /**< heartbeat check */
};


#if defined(SWIGJAVA) || defined(JAVACALLBACK)
#include "Callback.h"
#else
typedef void (* callback_handler) (clbk_data);
#endif

/*!
 * @class Pulsensor
 * @brief The context for the heartbeat pulse sensor
 */
class Pulsensor {

public:
#if defined(SWIGJAVA) || defined(JAVACALLBACK)
    Pulsensor(Callback *callback);
#else
    Pulsensor(callback_handler handler);
#endif
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
    uint8_t          pin; /**< Pin */
    uint8_t          ret; /**< Return value */
    mraa::Aio        pin_ctx; /**< The pin context */
#if defined(SWIGJAVA) || defined(JAVACALLBACK)
    Callback *obj_callback; /**< The callback object */
#else
    callback_handler callback; /**< The callback function */
#endif
    volatile uint16_t ctx_counter;
};

