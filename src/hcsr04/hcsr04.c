/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Author: Rafael Neri <rafael.neri@gmail.com>
 * Author: Jun Kato <i@junkato.jp>
 * Contributions by: Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2014-2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#include "upm_utilities.h"
#include "hcsr04.h"

hcsr04_context hcsr04_init(int triggerPin, int echoPin) {
    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        return NULL;
    }

    hcsr04_context dev = (hcsr04_context) malloc(sizeof(struct _hcsr04_context));

    if(!dev) {
        return NULL;
    }

    // initialize the GPIO pins
    dev->trigPin = mraa_gpio_init(triggerPin);
    if(!dev->trigPin) {
        printf("Unable to initialize the trigger pin\n");
        free(dev);
        return NULL;
    }

    dev->echoPin = mraa_gpio_init(echoPin);
    if(!dev->echoPin) {
        printf("Unable to initialize the echo pin\n");
        free(dev);
        return NULL;
    }

    // Setting direction for the GPIO pins
    if(mraa_gpio_dir(dev->trigPin, MRAA_GPIO_OUT) != MRAA_SUCCESS) {
        printf("Unable to set the direction of the trigger Pin\n");
        free(dev);
        return NULL;
    }

    if(mraa_gpio_dir(dev->echoPin, MRAA_GPIO_IN) != MRAA_SUCCESS) {
        printf("Unable to set the direction of the echo Pin\n");
        free(dev);
        return NULL;
    }

    // Setting the trigger pin to logic level 0
    if(mraa_gpio_write(dev->trigPin, 0) != MRAA_SUCCESS) {
        free(dev);
        return NULL;
    }

    // initialize the interrupt counter
    dev->interruptCounter = 0;

    return dev;
}

void hcsr04_close(hcsr04_context dev) {
    mraa_gpio_close(dev->trigPin);
    mraa_gpio_close(dev->echoPin);
    free(dev);
}

double hcsr04_get_distance(hcsr04_context dev, HCSR04_U unit) {
    // set value to start cycle right after trigger
    long cycleLength = 0, sampleTime = 0;
    struct timeval tv;
    int reading = 0;

    dev->interruptCounter = 0;
    gettimeofday(&tv, NULL);

    // The datasheet suggests using cycles of upto 60 ms
    // being a little libteral and using 70 ms, though a limit
    // should not be necessary at all
    cycleLength = (1000000 * tv.tv_sec) + tv.tv_usec + 70000;
    mraa_gpio_write(dev->trigPin, 1);
    upm_delay_us(10);
    mraa_gpio_write(dev->trigPin, 0);

    while(sampleTime < cycleLength) {
        reading = mraa_gpio_read(dev->echoPin);
        if(reading == 1 && dev->interruptCounter == 0) {
            gettimeofday(&tv, NULL);
            dev->startTime = (1000000 * tv.tv_sec) + tv.tv_usec;
            dev->interruptCounter++;
        } else if(reading == 0 && dev->interruptCounter == 1) {
            gettimeofday(&tv, NULL);
            dev->endTime = (1000000 * tv.tv_sec) + tv.tv_usec;
            break;
        } else {
            sampleTime = (1000000 * tv.tv_sec) + tv.tv_usec;
        }
    }

    if(unit == HCSR04_CM)
        return ((dev->endTime - dev->startTime)/2)/29.1;
    else
        return ((dev->endTime - dev->startTime)/2)/74.1;
}
