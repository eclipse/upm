/*
 * Author: Jon Trulson <jtrulson@ics.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#include "a110x.h"

a110x_context a110x_init(int pin) {
    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        return NULL;
    }

    a110x_context dev =
      (a110x_context) malloc(sizeof(struct _a110x_context));

    if (!dev)
      {
        return NULL;
      }

    dev->gpio_pin = pin;
    dev->gpio = mraa_gpio_init(dev->gpio_pin);
    if (mraa_gpio_dir(dev->gpio, MRAA_GPIO_IN) != MRAA_SUCCESS){
        free(dev);
        return NULL;
    }
    dev->isr_installed = false;
    return dev;
}

void a110x_close(a110x_context dev){
    free(dev);
}

upm_result_t a110x_magnet_detected(a110x_context dev, bool* res){
    int val = mraa_gpio_read(dev->gpio);
    if (val == 0)
        *res = false;
    else
        *res = true;
    return UPM_SUCCESS;
}

upm_result_t a110x_install_isr(a110x_context dev,
                               mraa_gpio_edge_t edge_level,
                               void (*isr)(void *), void *arg){
    if (dev->isr_installed)
        a110x_uninstall_isr(dev);

    mraa_gpio_isr(dev->gpio, edge_level, isr, arg);
    dev->isr_installed = true;
    return UPM_SUCCESS;
}

upm_result_t a110x_uninstall_isr(a110x_context dev){
    mraa_gpio_isr_exit(dev->gpio);
    dev->isr_installed = false;
    return UPM_SUCCESS;
}
