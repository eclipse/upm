/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2014 Intel Corporation.
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
#include "es9257.h"

es9257_context es9257_init(int32_t pin, int32_t min_pulse_width,
                         int32_t max_pulse_width) {

    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        return NULL;
    }

    es9257_context dev = (es9257_context) malloc(sizeof(struct _es9257_context));

    if(dev == NULL){
        printf("Unable to assign memory to the Servo motor structure");
        return NULL;
    }

    dev->servo_pin = pin;

    // second is the min pulse width
    dev->min_pulse_width = min_pulse_width;
    // third is the max pulse width
    dev->max_pulse_width = max_pulse_width;

    dev->pwm = mraa_pwm_init(dev->servo_pin);
    if(dev->pwm == NULL){
        printf("Unable to initialize the PWM pin");
    }

    es9257_set_angle(dev, 0);
    return dev;
}

void es9257_halt(es9257_context dev){
    mraa_pwm_enable(dev->pwm, 0);
    free(dev);
}

upm_result_t es9257_set_angle(es9257_context dev, int32_t angle){

    if(ES9257_MAX_ANGLE < angle || angle < 0){
        printf("The angle specified is either above the max angle or below 0");
        return UPM_ERROR_UNSPECIFIED;
    }
    printf("setting angle to: %d\n", angle);

    mraa_pwm_enable(dev->pwm, 1);
    mraa_pwm_period_us(dev->pwm, ES9257_PERIOD);
    int32_t val = 0;

    es9257_calc_pulse_travelling(dev, &val, angle);
    mraa_pwm_pulsewidth_us(dev->pwm, val);

    upm_delay(1);
    mraa_pwm_enable(dev->pwm, 0);

    return UPM_SUCCESS;
}

upm_result_t es9257_calc_pulse_travelling(const es9257_context dev,
                                         int32_t* ret_val, int32_t value){
    if (value > dev->max_pulse_width) {
        return dev->max_pulse_width;
    }

    // if less than the boundaries
    if (value  < 0) {
        return dev->min_pulse_width;
    }

    *ret_val = (int) ((float)dev->min_pulse_width + ((float)value / ES9257_MAX_ANGLE) * ((float)dev->max_pulse_width - (float)dev->min_pulse_width));
    return UPM_SUCCESS;
}

void es9257_set_min_pulse_width (es9257_context dev, int width){
    dev->min_pulse_width = width;
}

void es9257_set_max_pulse_width (es9257_context dev, int width){
    dev->max_pulse_width = width;
}

int es9257_get_min_pulse_width (es9257_context dev){
    return dev->min_pulse_width;
}

int es9257_get_max_pulse_width (es9257_context dev){
    return dev->max_pulse_width;
}
