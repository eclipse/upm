/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
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
#include <stdio.h>
#include <upm.h>

#include <mraa/gpio.h>

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @file rpr220.h
     * @library rpr220
     * @brief C API for the rpr220 driver
     *
     * @include rpr220.c
     */

    /**
     * Device context
     */
    typedef struct _rpr220_context {
        mraa_gpio_context gpio;

        bool isrInstalled;
    } *rpr220_context;

    /**
     * RPR220 initialization.
     *
     * @param pin Digital pin to use.
     * @return rpr220 device context
     */
    rpr220_context rpr220_init(int pin);

    /**
     * RPR220 destructor
     *
     * @param dev Device context.
     */
    void rpr220_close(rpr220_context dev);

    /**
     * Gets the status of the pin; true means black has been detected
     *
     * @param dev Device context.
     * @return True if the sensor has detected black
     */
    bool rpr220_black_detected(const rpr220_context dev);

    /**
     * Installs an ISR to be called when
     * black is detected
     *
     * @param dev Device context.
     * @param isr Pointer to a function to be called on interrupt
     * @param arg Pointer to an object to be supplied as an
     * argument to the ISR.
     */
    void rpr220_install_isr(const rpr220_context dev,
                            void (*isr)(void *), void *arg);

    /**
     * Uninstalls the previously installed ISR
     *
     * @param dev Device context.
     */
    void rpr220_uninstall_isr(const rpr220_context dev);


#ifdef __cplusplus
}
#endif
