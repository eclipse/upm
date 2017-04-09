/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * These modules were rewritten, based on original work by:
 *
 * (original my9221/groveledbar)
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * (grovecircularled)
 * Author: Jun Kato and Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Contributions: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
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
     * @file my9221.h
     * @library my9221
     * @brief C API for the my9221 driver
     *
     * @include my9221.c
     */

    /**
     * Device context
     */
    typedef struct _my9221_context {
        mraa_gpio_context gpioClk;
        mraa_gpio_context gpioData;

        bool              autoRefresh;
        // we're only doing 8-bit greyscale, so the high order bits are
        // always 0
        uint16_t          lowIntensity;
        uint16_t          highIntensity;

        unsigned int      instances;
        unsigned int      maxLEDS;

        // an array of uint16_t's representing our bit states (on/off)
        // intensities.  Only the low 8 bits are used, but in the future
        // 16bit support can work here as well.
        uint16_t          *bitStates;

        uint16_t          commandWord;

        bool              initialized;

        // A helper for the users of this driver
        unsigned int      max_leds_per_instance;
    } *my9221_context;


    /**
     * Instantiates an MY9221 object
     *
     * @param dataPin Data pin
     * @param clockPin Clock pin
     * @param instances Number of daisy-chained my9221s, must be at
     * least 1
     * @return Device context
     */
    my9221_context my9221_init(uint8_t dataPin, uint8_t clockPin,
                               int instances);

    /**
     * MY9221 close
     *
     * @param dev Device context
     */
    void my9221_close(my9221_context dev);

    /**
     * Enable or disable auto refresh.  When auto refresh is enabled,
     * update the LED display as soon as the internal state changes.
     * When false, the display(s) will not be updated until the
     * refresh() method is called.
     *
     * @param dev Device context
     * @param enable true to enable auto refresh, false otherwise
     */
    void my9221_set_auto_refresh(const my9221_context dev, bool enable);

    /**
     * Set an LED to a specific on (high intensity) or off (low
     * intensity) value.
     *
     * @param dev Device context
     * @param led The LED whose state you wish to change
     * @param on true to turn on the LED, false to turn the LED off
     */
    void my9221_set_led(const my9221_context dev, int led, bool on);

    /**
     * Set the greyscale intensity of an LED in the OFF state.  The
     * intensity is a value from 0 (fully off) to 255 (fully on).
     * This will take effect on any future LED set or clear
     * operations.
     *
     * @param dev Device context
     * @param intensity a value from 0 (fully off) to 255 (fully on)
     */
    void my9221_set_low_intensity_value(const my9221_context dev,
                                        int intensity);

    /**
     * Set the greyscale intensity of an LED in the ON state.  The
     * intensity is a value from 0 (fully off) to 255 (fully on).
     * This will take effect on any future LED set or clear
     * operations.
     *
     * @param dev Device context
     * @param intensity a value from 0 (fully off) to 255 (fully on)
     */
    void my9221_set_high_intensity_value(const my9221_context dev,
                                         int intensity);

    /**
     * Set all of the LEDS to the ON (high intensity value) state.
     *
     * @param dev Device context
     */
    void my9221_set_all(const my9221_context dev);

    /**
     * Set all of the LEDS to the OFF (low intensity value) state.
     *
     * @param dev Device context
     */
    void my9221_clear_all(const my9221_context dev);

    /**
     * Set the LED states to match the internal stored states.  This
     * is useful when auto refresh (setAutoRefresh()) is false to
     * update the display.
     *
     * @param dev Device context
     */
    void my9221_refresh(const my9221_context dev);

    /**
     * Return the maximum number of LEDs present, based on the number
     * of instances specified when the device context was initialized.
     *
     * @param dev Device context
     * @return The number of LEDs that can be controlled.
     */
    int my9221_get_max_leds(const my9221_context dev);

#ifdef __cplusplus
}
#endif
