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
#include "gp2y0a.h"

gp2y0a_context gp2y0a_init(int pin, float a_ref){
    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        return NULL;
    }

    gp2y0a_context dev =
        (gp2y0a_context) malloc(sizeof(struct _gp2y0a_context));

    if (!dev)
    {
        printf("%s: device context allocation failed.\n",
               __FUNCTION__);
        return NULL;
    }

    memset((void *)dev, 0, sizeof(struct _gp2y0a_context));
    dev->pin = pin;
    dev->aio = mraa_aio_init(dev->pin);
    if(dev->aio == NULL){
        free(dev);
        return NULL;
    }
    dev->a_res = (1 << mraa_aio_get_bit(dev->aio));

    return dev;
}

void gp2y0a_close(gp2y0a_context dev){
    free(dev);
}

upm_result_t gp2y0a_get_value(gp2y0a_context dev, float a_ref,
                              uint8_t samples, float* value){
    int val;
    int sum = 0;
    int i = 0;

    if (samples <= 0)
      samples = 1;

    for(i=0; i<samples; i++) {
        val = mraa_aio_read(dev->aio);
        sum += val;
    }
    val = sum/samples;
    float volts = (float)(val * a_ref) / (float)dev->a_res;
    *value = volts;

    return UPM_SUCCESS;
}
