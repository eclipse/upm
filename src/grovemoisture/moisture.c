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

#include "moisture.h"

grovemoisture_context grovemoisture_init(int pin) {
    grovemoisture_context dev =
      (grovemoisture_context) malloc(sizeof(struct _grovemoisture_context));

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

void grovemoisture_close(grovemoisture_context dev) {
    mraa_aio_close(dev->aio);
    free(dev);
}

upm_result_t grovemoisture_get_moisture(grovemoisture_context dev,
                                            int* moisture) {

    *moisture = mraa_aio_read(dev->aio);

    return UPM_SUCCESS;
}
