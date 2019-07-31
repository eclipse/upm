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

#include "ad8232.h"

ad8232_context ad8232_init(int lo_plus, int lo_minus, int output,
                           float a_ref) {
    ad8232_context dev =
      (ad8232_context) malloc(sizeof(struct _ad8232_context));

    if(dev == NULL){
        printf("Unable to assign memory to the Heart Rate Monitor structure");
        return NULL;
    }

    dev->aio = mraa_aio_init(output);
    dev->gpio_lo_plus = mraa_gpio_init(lo_plus);
    dev->gpio_lo_minus = mraa_gpio_init(lo_minus);

    if (dev->aio == NULL || dev->gpio_lo_minus == NULL ||
        dev->gpio_lo_plus == NULL){
        printf("The pins did not initialize correctly");
        return NULL;
    }

    if (mraa_gpio_dir(dev->gpio_lo_minus, MRAA_GPIO_IN) != MRAA_SUCCESS ||
        mraa_gpio_dir(dev->gpio_lo_plus, MRAA_GPIO_IN) != MRAA_SUCCESS){
        printf("Couldn't set the direction for the GPIO pins");
        return NULL;
    }

    dev->a_res = (1 << mraa_aio_get_bit(dev->aio));
    return dev;
}

void ad8232_close(ad8232_context dev){
    if (dev->gpio_lo_minus != NULL)
        mraa_gpio_close(dev->gpio_lo_minus);
    if (dev->gpio_lo_plus != NULL)
        mraa_gpio_close(dev->gpio_lo_plus);
    if (dev->aio != NULL)
        mraa_aio_close(dev->aio);

    free(dev);
}

upm_result_t ad8232_get_value(ad8232_context dev, int* value) {
    int reading = 0;

    if (mraa_gpio_read(dev->gpio_lo_minus) ||
        mraa_gpio_read(dev->gpio_lo_plus)) {
        reading = 0;
    }
    else {
        reading = mraa_aio_read(dev->aio);
    }

    *value = reading;

    return UPM_SUCCESS;
}
