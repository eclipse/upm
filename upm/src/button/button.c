/*
 * Authors: Brendan Le Foll <brendan.le.foll@intel.com>
 *          Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 *          Sarah Knepper <sarah.knepper@intel.com>
 *          Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2014 - 2016 Intel Corporation.
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

#include "button.h"

button_context button_init(int pin){
    // make sure that mraa is initialized
    int mraa_rv;
    if((mraa_rv = mraa_init()) != MRAA_SUCCESS){
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        return NULL;
    }

    button_context dev = (button_context) malloc(sizeof(struct _button_context));

    if (dev == NULL){
        printf("Unable to allocate space for the sensor structure\n");
        return NULL;
    }

    dev->gpio = mraa_gpio_init(pin);
    mraa_gpio_dir(dev->gpio, MRAA_GPIO_IN);
    dev->isr_installed = false;

    return dev;
}

void button_close(button_context dev){
    mraa_gpio_close(dev->gpio);
    free(dev);
}

upm_result_t button_get_value(button_context dev, bool* val){
    int ret = mraa_gpio_read(dev->gpio);

    if(ret > 0)
        *val = true;
    else
        *val = false;

    return UPM_SUCCESS;
}

upm_result_t button_install_isr(button_context dev, mraa_gpio_edge_t edge_level, void (*isr)(void *), void *arg){
    button_uninstall_isr(dev);

    mraa_gpio_isr(dev->gpio, edge_level, isr, arg);
    dev->isr_installed = true;

    return UPM_SUCCESS;
}

upm_result_t button_uninstall_isr(button_context dev){
    if(dev->isr_installed)
        mraa_gpio_isr_exit(dev->gpio);

    dev->isr_installed = false;

    return UPM_SUCCESS;
}
