/*
 * Author: Sergey Kiselev <sergey.kiselev@intel.com>
 * Copyright (c) 2017 Sergey Kiselev
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <string.h>
#include <assert.h>

#include "p9813.h"
#include "upm_utilities.h"

p9813_context p9813_init(int ledcount, int clk, int data) {

    p9813_context dev = (p9813_context)malloc(sizeof(struct _p9813_context));

    if(!dev)
        return NULL;

    memset(dev, 0, sizeof(struct _p9813_context));
    dev->leds = ledcount;

    // Try to allocate and zero out buffer
    if ((dev->buffer = (uint8_t*)malloc(dev->leds * 3))) {
        memset(dev->buffer, 0x00, dev->leds * 3);
    } else {
        printf("%s: Failed to allocate LED buffer.\n", __FUNCTION__);
        p9813_close(dev);
        return NULL;
    }

    dev->clk = NULL;
    dev->data = NULL;

    if (!(dev->clk = mraa_gpio_init(clk))) {
        printf("%s: mraa_gpio_init(clk) failed.\n", __FUNCTION__);
        p9813_close(dev);
        return NULL;
    }
    mraa_gpio_dir(dev->clk, MRAA_GPIO_OUT);

    if (!(dev->data = mraa_gpio_init(data))) {
        printf("%s: mraa_gpio_init(data) failed.\n", __FUNCTION__);
        p9813_close(dev);
        return NULL;
    }

    return dev;
}

void p9813_close(p9813_context dev) {
    assert(dev != NULL);

    if (dev->clk)
        mraa_gpio_close(dev->clk);
    if (dev->data)
        mraa_gpio_close(dev->data);
    if(dev->buffer)
        free(dev->buffer);
    free(dev);
}

upm_result_t p9813_set_led(p9813_context dev, uint16_t index, uint8_t r, uint8_t g, uint8_t b) {
    return p9813_set_leds(dev, index, index, r, g, b);
}

upm_result_t p9813_set_leds(p9813_context dev, uint16_t s_index, uint16_t e_index, uint8_t r, uint8_t g, uint8_t b) {
    assert(dev != NULL);
    int i;

    for (i = s_index; i <= e_index; i++) {
        dev->buffer[i * 3] = r;
        dev->buffer[i * 3 + 1] = g;
        dev->buffer[i * 3 + 2] = b;
    }
    return UPM_SUCCESS;
}

upm_result_t p9813_send_byte(p9813_context dev, uint8_t data)
{
    for (uint16_t i = 0; i < 8; i++) {
        // Write the data bit
        mraa_gpio_write(dev->data, (data & 0x80) >> 7);
        // Pulse the clock
        mraa_gpio_write(dev->clk, 1);
        upm_delay_us(20);
        mraa_gpio_write(dev->clk, 0);
        upm_delay_us(20);

        // Shift to the next data bit
        data <<= 1;
    }

    return UPM_SUCCESS;
}


upm_result_t p9813_refresh(p9813_context dev) {
    uint16_t i;
    uint8_t byte0, red, green, blue;

    assert(dev != NULL);

    // Begin data frame
    p9813_send_byte(dev, 0x00);
    p9813_send_byte(dev, 0x00);
    p9813_send_byte(dev, 0x00);
    p9813_send_byte(dev, 0x00);

    for (i = 0; i <= dev->leds; i++) {
        red = dev->buffer[i * 3];
        green = dev->buffer[i * 3 + 1];
        blue = dev->buffer[i * 3 + 2];
        // The first byte is 11 followed by inverted bits 7 and 6 of blue, green, and red
        byte0 = 0xFF;
        byte0 ^= (blue >> 2) & 0x30; // XOR bits 4-5
        byte0 ^= (green >> 4) & 0x0C; // XOR bits 2-3
        byte0 ^= (red >> 6) & 0x03; // XOR bits 0-1
        p9813_send_byte(dev, byte0);
        p9813_send_byte(dev, blue);
        p9813_send_byte(dev, green);
        p9813_send_byte(dev, red);
    }

    // End data frame
    p9813_send_byte(dev, 0x00);
    p9813_send_byte(dev, 0x00);
    p9813_send_byte(dev, 0x00);
    p9813_send_byte(dev, 0x00);

    return UPM_SUCCESS;
}
