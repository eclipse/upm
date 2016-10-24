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
     * @file uln200xa.h
     * @library uln200xa
     * @brief C API for the uln200xa driver
     *
     * @include uln200xa.c
     */

    /**
     * Device context
     */
    typedef struct _uln200xa_context {
        mraa_gpio_context stepI1; // In1
        mraa_gpio_context stepI2;
        mraa_gpio_context stepI3;
        mraa_gpio_context stepI4;

        int      stepsPerRev;
        int      currentStep;
        uint32_t stepDelay;
        int      stepDirection;

    } *uln200xa_context;

    /**
     * Enum to specify the direction of a motor
     */
    typedef enum {
      ULN200XA_DIR_CW   = 0x01,
      ULN200XA_DIR_CCW  = 0x02
    } ULN200XA_DIRECTION_T;

    /**
     * ULN200XA constructor
     *
     * @param stepsPerRev Number of steps per full revolution
     * @param i1 Digital pin to use for stepper input 1
     * @param i2 Digital pin to use for stepper input 2
     * @param i3 Digital pin to use for stepper input 3
     * @param i4 Digital pin to use for stepper input 4
     * @return Device context
     */
    uln200xa_context uln200xa_init(int stepsPerRev, unsigned int i1,
                                   unsigned int i2, unsigned int i3,
                                   unsigned int i4);

    /**
     * ULN200XA destructor
     */
    void uln200xa_close(uln200xa_context dev);

    /**
     * Sets the speed of the stepper motor in revolutions per minute (RPM)
     *
     * @param dev Device context
     * @param speed Speed to set the motor to, in RPM
     */
    void uln200xa_set_speed(const uln200xa_context dev, unsigned int speed);

    /**
     * Sets the direction of the motor, clockwise or counterclockwise
     *
     * @param dev Device context
     * @param dir Direction to set the motor to
     */
    void uln200xa_set_direction(const uln200xa_context dev,
                                ULN200XA_DIRECTION_T dir);

    /**
     * Steps the stepper motor a specified number of steps
     *
     * @param dev Device context
     * @param steps Number of steps to move the stepper motor
     */
    void uln200xa_stepper_steps(const uln200xa_context dev, unsigned int steps);

    /**
     * Releases the stepper motor by removing power
     *
     * @param dev Device context
     */
    void uln200xa_release(const uln200xa_context dev);

#ifdef __cplusplus
}
#endif
