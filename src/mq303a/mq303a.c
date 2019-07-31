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

#include "mq303a.h"
#include <stdlib.h>

mq303a_context mq303a_init(int pin, int heater_pin) {
    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        return NULL;
    }

    mq303a_context dev =
      (mq303a_context)malloc(sizeof(struct _mq303a_context));

    if (!dev)
      return NULL;

    dev->aio = mraa_aio_init(pin);
    dev->gpio = mraa_gpio_init(heater_pin);

    if(mraa_gpio_dir(dev->gpio, MRAA_GPIO_OUT) != MRAA_SUCCESS)
      {
        free(dev);
        return NULL;
      }

    return dev;
}

void mq303a_close(mq303a_context dev){
    free(dev);
}

upm_result_t mq303a_heater_enable(mq303a_context dev, bool enable){
    if(enable)
        mraa_gpio_write(dev->gpio, 0);
    else
        mraa_gpio_write(dev->gpio, 1);

    return UPM_SUCCESS;
}

upm_result_t mq303a_get_value(mq303a_context dev, int* val){
    *val = mraa_aio_read(dev->aio);

    if (*val < 0)
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}
