/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Contributions: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Contributions: Sarah Knepper <sarah.knepper@intel.com>
 *                Abhishek Malik <abhishek.malik@intel.com>
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
#include "led.h"
#include "upm_types.h"

led_context led_init(int pin){
    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        return NULL;
    }

    led_context dev =
      (led_context)malloc(sizeof(struct _led_context));

    if (!dev)
        return NULL;

    dev->name = NULL;
    dev->gpioled = NULL;

    dev->led_pin = pin;
    dev->gpio = mraa_gpio_init(dev->led_pin);

    if (mraa_gpio_dir(dev->gpio, MRAA_GPIO_OUT) != MRAA_SUCCESS) {
        printf("%s: Unable to set pin %d as output.\n", __FUNCTION__, pin);
        free(dev);
        return NULL;
    }

    return dev;
}

led_context led_init_str(const char* name){
    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        return NULL;
    }

    led_context dev =
      (led_context)malloc(sizeof(struct _led_context));

    if (!dev)
        return NULL;

    dev->led_pin = -1;
    dev->gpio = NULL;

    dev->name = name;
    dev->gpioled = mraa_led_init_raw(name);

    if (!dev->gpioled) {
        printf("%s: Unable to initialize gpioled device (%s).\n", __FUNCTION__, dev->name);
        free(dev);
        return NULL;
    }

    dev->max_brightness = mraa_led_read_max_brightness(dev->gpioled);

    return dev;
}

void led_close(led_context dev){
    if (dev->gpio) {
        mraa_gpio_close(dev->gpio);
    }
    if (dev->gpioled) {
        mraa_led_close(dev->gpioled);
    }
    free(dev);
}

upm_result_t led_on(led_context dev){
    if (dev->gpio) {
        // using gpio pin
        if (mraa_gpio_write(dev->gpio, 1) != MRAA_SUCCESS)
            return UPM_ERROR_OPERATION_FAILED;
    } else {
        // using gpioled device
        if (mraa_led_set_brightness(dev->gpioled, dev->max_brightness) != MRAA_SUCCESS)
            return UPM_ERROR_OPERATION_FAILED;
    }
    return UPM_SUCCESS;
}

upm_result_t led_off(led_context dev){
    if (dev->gpio) {
        // using gpio pin
        if (mraa_gpio_write(dev->gpio, 0) != MRAA_SUCCESS)
            return UPM_ERROR_OPERATION_FAILED;
    } else {
        // using gpioled device
        if (mraa_led_set_brightness(dev->gpioled, 0) != MRAA_SUCCESS)
            return UPM_ERROR_OPERATION_FAILED;
    }
    return UPM_SUCCESS;
}
