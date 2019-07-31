/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once


#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include <upm.h>
#include <upm_utilities.h>
#include <mraa/gpio.h>

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @file wfs.h
     * @library wfs
     * @brief WFS (Water Flow Sensor)
     *
     */

    /**
     * Device context
     */
    typedef struct _wfs_context {
        mraa_gpio_context gpio;

        volatile uint32_t flowCounter;
        upm_clock_t       clock;
        bool              isrInstalled;
    } *wfs_context;

    /**
     * Initialize a WFS device
     *
     * @param pin GPIO pin to use.  This must be an interrupt capable
     * pin.
     * @return Device context, or NULL on error
     */
    wfs_context wfs_init(int pin);

    /**
     * WFS close function
     *
     * @param dev Device context
     */
    void wfs_close(wfs_context dev);

    /**
     * Returns the number of milliseconds elapsed since wfs_init_clock()
     * was last called.
     *
     * @param dev Device context
     * @return Elapsed milliseconds
     */
    uint32_t wfs_get_millis(const wfs_context dev);

    /**
     * Resets the clock
     *
     * @param dev Device context
     */
    void wfs_init_clock(const wfs_context dev);

    /**
     * Resets the flow counter to 0. The flow counter should be
     * stopped via stopFlowCounter() prior to calling this function.
     *
     * @param dev Device context
     */
    void wfs_clear_flow_counter(const wfs_context dev);

    /**
     * Starts the flow counter
     *
     * @param dev Device context
     * @return UPM result
     */
    upm_result_t wfs_start_flow_counter(const wfs_context dev);

    /**
     * Stops the flow counter
     *
     * @param dev Device context
     */
    void wfs_stop_flow_counter(const wfs_context dev);

    /**
     * Gets the flow counter
     *
     * @param dev Device context
     * @return Flow counter
     */
    uint32_t wfs_flow_counter(const wfs_context dev);

    /**
     * Computes the flow rate in liters per minute (LPM).  Note, this
     * is for the Grove WFS.  If you are using some other WFS, you
     * should compute the flow rate on your own based on the data for
     * your sensor.
     *
     * @param dev Device context
     * @return Computed flow rate in liters per minute
     */
    float wfs_flow_rate(const wfs_context dev);


#ifdef __cplusplus
}
#endif
