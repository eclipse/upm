/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014-2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "water.h"

water_context water_init(unsigned int pin)
{
  water_context dev =
    (water_context)malloc(sizeof(struct _water_context));

  if (!dev)
    return NULL;

  dev->gpio = NULL;

  // make sure MRAA is initialized
  int mraa_rv;
  if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
  {
      printf("mraa_init() failed (%d).\n", mraa_rv);
      water_close(dev);
      return NULL;
  }

  // initialize the MRAA context

  if (!(dev->gpio = mraa_gpio_init(pin)))
    {
      printf("%s: mraa_gpio_init() failed.\n", __FUNCTION__);
      water_close(dev);
      return NULL;
    }

  mraa_gpio_dir(dev->gpio, MRAA_GPIO_IN);

  return dev;
}

void water_close(const water_context dev)
{
  assert(dev != NULL);

  if (dev->gpio)
    mraa_gpio_close(dev->gpio);

  free(dev);
}

bool water_is_wet(const water_context dev)
{
  assert(dev != NULL);

  // gpio is low when wet
  return (mraa_gpio_read(dev->gpio) ? false : true);
}
