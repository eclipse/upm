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
     * Device context
     */
    typedef struct _linefinder_context {
        mraa_gpio_context gpio;
    } *linefinder_context;

    /**
     *  Line Finder inititialization
     *
     * @param pin Digital pin to use
     * @return device context
     */
    linefinder_context linefinder_init(int pin);

    /**
     * LineFinder close
     *
     * @param Device context
     */
    void linefinder_close(linefinder_context dev);

    /**
     * Determines whether white has been detected
     *
     * @param Device context
     * @return True if white is detected
     */
    bool linefinder_white_detected(const linefinder_context dev);

    /**
     * Determines whether black has been detected
     *
     * @param Device context
     * @return True if black is detected
     */
    bool linefinder_black_detected(const linefinder_context dev);


#ifdef __cplusplus
}
#endif
