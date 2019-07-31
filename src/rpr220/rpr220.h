/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
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
