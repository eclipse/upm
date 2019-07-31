/*
 * Author: Zion Orent <sorent@ics.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#include "collision.h"

collision_context collision_init(int pin) {
    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        return NULL;
    }

    collision_context dev =
      (collision_context)malloc(sizeof(struct _collision_context));

    if (!dev)
      return NULL;

    dev->gpio = mraa_gpio_init(pin);

    if(mraa_gpio_dir(dev->gpio, MRAA_GPIO_IN) != MRAA_SUCCESS)
      {
        free(dev);
        return NULL;
      }

    return dev;
}

void collision_close(collision_context dev){
    free(dev);
}

upm_result_t collision_is_colliding(collision_context dev, bool* collision_val){
    int value = mraa_gpio_read(dev->gpio);

    if(!value)
        *collision_val = true;
    else
        *collision_val = false;

    return UPM_SUCCESS;
}
