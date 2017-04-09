/*
 * Author: Jon Trulson <jtrulson@ics.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
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

#include "ad8232.h"

ad8232_context ad8232_init(int lo_plus, int lo_minus, int output,
                           float a_ref) {
    ad8232_context dev =
      (ad8232_context) malloc(sizeof(struct _ad8232_context));

    if(dev == NULL){
        printf("Unable to assign memory to the Heart Rate Monitor structure");
        return NULL;
    }

    dev->gpio_lo_plus_pin = lo_plus;
    dev->gpio_lo_minus_pin = lo_minus;
    dev->aio_pin = output;

    dev->aio = mraa_aio_init(dev->aio_pin);
    dev->gpio_lo_plus = mraa_gpio_init(dev->gpio_lo_plus_pin);
    dev->gpio_lo_minus = mraa_gpio_init(dev->gpio_lo_minus_pin);

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
