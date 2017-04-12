/*
 * Author: Jon Trulson <jtrulson@ics.com>
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
