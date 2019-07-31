/*
 * Author: Sarah Knepper <sarah.knepper@intel.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include "ttp223.h"

ttp223_context ttp223_init(int pin) {
    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        return NULL;
    }

    ttp223_context dev = 
      (ttp223_context) malloc(sizeof(struct _ttp223_context));

    if (dev == NULL) {
        printf("Unable to allocate space for the sensor struct\n");
        return NULL;
    }

    dev->gpio = mraa_gpio_init(pin);
    mraa_gpio_dir(dev->gpio, MRAA_GPIO_IN);
    dev->isr_installed = false;

    return dev;
}

void ttp223_close(ttp223_context dev){
  mraa_gpio_close(dev->gpio);
  free(dev);
}

upm_result_t ttp223_is_pressed(ttp223_context dev, bool* value) {
    int ret = mraa_gpio_read(dev->gpio);

    if (ret > 0)
        *value = true;
    else
        *value = false;

    return UPM_SUCCESS;
}

upm_result_t upm_ttp223_install_isr(ttp223_context dev,
                                    mraa_gpio_edge_t edge_level,
                                    void (*isr)(void *), void *arg){
    ttp223_uninstall_isr(dev);

    mraa_gpio_isr(dev->gpio, edge_level, isr, arg);
    dev->isr_installed = true;

    return UPM_SUCCESS;
}

upm_result_t ttp223_uninstall_isr(ttp223_context dev){
    if (dev->isr_installed)
        mraa_gpio_isr_exit(dev->gpio);

    dev->isr_installed = false;

    return UPM_SUCCESS;
}
