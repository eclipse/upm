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
#include "loudness.h"

loudness_context loudness_init(int pin) {
    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        return NULL;
    }

    loudness_context dev =
      (loudness_context) malloc(sizeof(struct _loudness_context));

    if (!dev)
      return NULL;

    dev->aio = mraa_aio_init(pin);

    if (!dev->aio)
      {
        free(dev);
        return NULL;
      }

    return dev;
}

void loudness_close(loudness_context dev) {
    mraa_aio_close(dev->aio);
    free(dev);
}

upm_result_t loudness_get_value(loudness_context dev, int* val) {
    *val = mraa_aio_read(dev->aio);

    return UPM_SUCCESS;
}
