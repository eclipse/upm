/*
 * Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Copyright (c) 2016 Intel Corporation
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

#include <string.h>
#include <assert.h>

#include "apa102.h"

apa102_context apa102_init(int ledcount, int bus, int cs) {

    apa102_context dev = (apa102_context)malloc(sizeof(struct _apa102_context));

    if(!dev)
        return NULL;

    memset(dev, 0, sizeof(struct _apa102_context));
    dev->leds = ledcount;

    // Try to allocate and zero out buffer
    uint16_t endframelength = (dev->leds + 15) / 16;
    dev->framelength = 4 * (1 + dev->leds) + endframelength;
    if ((dev->buffer = (uint8_t*)malloc(dev->framelength))) {
        memset(dev->buffer, 0x00, dev->framelength - endframelength);
        memset(&dev->buffer[dev->framelength - endframelength], 0xFF, endframelength);
        int i;
        for (i = 1; i <= dev->leds; i++) {
            dev->buffer[i * 4] = 224;
        }
    } else {
        printf("%s: Failed to allocate LED buffer.\n", __FUNCTION__);
        apa102_close(dev);
        return NULL;
    }

    dev->spi = NULL;
    dev->cs = NULL;

    // Initialize MRAA contexts
    if (!(dev->spi = mraa_spi_init(bus))) {
        printf("%s: mraa_spi_init(bus) failed.\n", __FUNCTION__);
        apa102_close(dev);
        return NULL;
    }

    if (cs >= 0) {
        if (!(dev->cs = mraa_gpio_init(cs))) {
          printf("%s: mraa_gpio_init(cs) failed.\n", __FUNCTION__);
          apa102_close(dev);
          return NULL;
        }
        mraa_gpio_dir(dev->cs, MRAA_GPIO_OUT);
    }

    return dev;
}

void apa102_close(apa102_context dev) {
    assert(dev != NULL);

    if (dev->spi)
        mraa_spi_stop(dev->spi);
    if (dev->cs)
        mraa_gpio_close(dev->cs);
    if(dev->buffer)
        free(dev->buffer);
    free(dev);
}

upm_result_t apa102_set_led(apa102_context dev, uint16_t index, uint8_t brightness, uint8_t r, uint8_t g, uint8_t b) {
    return apa102_set_leds(dev, index, index, brightness, r, g, b);
}

upm_result_t apa102_set_led_brightness(apa102_context dev, uint16_t index, uint8_t brightness) {
    return apa102_set_leds_brightness(dev, index, index, brightness);
}

upm_result_t apa102_set_leds(apa102_context dev, uint16_t s_index, uint16_t e_index, uint8_t brightness, uint8_t r, uint8_t g, uint8_t b) {
    assert(dev != NULL);
    int i;
    uint16_t s_idx = (s_index + 1) * 4;
    uint16_t e_idx = (e_index + 1) * 4;

    for (i = s_idx; i <= e_idx; i += 4) {
        dev->buffer[i] = brightness | 224;
        dev->buffer[i + 1] = b;
        dev->buffer[i + 2] = g;
        dev->buffer[i + 3] = r;
    }
    return UPM_SUCCESS;
}

upm_result_t apa102_set_leds_brightness(apa102_context dev, uint16_t s_index, uint16_t e_index, uint8_t brightness) {
    assert(dev != NULL);
    int i;
    uint16_t s_idx = (s_index + 1) * 4;
    uint16_t e_idx = (e_index + 1) * 4;

    for (i = s_idx; i <= e_idx; i += 4) {
        dev->buffer[i] = brightness | 224;
    }
    return UPM_SUCCESS;
}

upm_result_t apa102_refresh(apa102_context dev) {
    assert(dev != NULL);
    if(!dev->cs) {
        mraa_spi_write_buf(dev->spi, dev->buffer, dev->framelength);
    } else {
        mraa_gpio_write(dev->cs, 1);
        mraa_spi_write_buf(dev->spi, dev->buffer, dev->framelength);
        mraa_gpio_write(dev->cs, 0);
    }
    return UPM_SUCCESS;
}
