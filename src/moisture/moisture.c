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

#include "moisture.h"

moisture_context moisture_init(int pin) {
    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        return NULL;
    }

    moisture_context dev =
      (moisture_context) malloc(sizeof(struct _moisture_context));

    if (dev == NULL) {
        printf("Unable to allocate memory for device context\n");
        return NULL;
    }

    dev->analog_pin = pin;
    dev->aio = mraa_aio_init(dev->analog_pin);

    if (dev->aio == NULL) {
        printf("mraa_aio_init() failed.\n");
        free(dev);

        return NULL;
    }

    return dev;
}

void moisture_close(moisture_context dev) {
    mraa_aio_close(dev->aio);
    free(dev);
}

upm_result_t moisture_get_moisture(moisture_context dev,
                                            int* moisture) {

    *moisture = mraa_aio_read(dev->aio);

    return UPM_SUCCESS;
}
