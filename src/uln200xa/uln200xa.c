/*
 * Author: Jon Trulson <jtrulson@ics.com>
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


#include <assert.h>
#include <upm_utilities.h>

#include "uln200xa.h"

static void uln200xa_stepper_step(const uln200xa_context dev);

uln200xa_context uln200xa_init(int stepsPerRev, unsigned int i1,
                               unsigned int i2, unsigned int i3,
                               unsigned int i4)
{
    uln200xa_context dev =
        (uln200xa_context)malloc(sizeof(struct _uln200xa_context));

    if (!dev)
        return NULL;

    dev->stepI1 = NULL;
    dev->stepI2 = NULL;
    dev->stepI3 = NULL;
    dev->stepI4 = NULL;

    dev->stepsPerRev = stepsPerRev;
    dev->currentStep = 0;
    dev->stepDelay = 0;
    dev->stepDirection = 1;          // default is forward

    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        uln200xa_close(dev);
        return NULL;
    }

    // MRAA contexts...
    if ( !(dev->stepI1 = mraa_gpio_init(i1)) )
    {
        printf("%s: mraa_gpio_init(i1) failed\n",
               __FUNCTION__);
        uln200xa_close(dev);
        return NULL;
    }
    mraa_gpio_dir(dev->stepI1, MRAA_GPIO_OUT);

    if ( !(dev->stepI2 = mraa_gpio_init(i2)) )
    {
        printf("%s: mraa_gpio_init(i2) failed\n",
               __FUNCTION__);
        uln200xa_close(dev);
        return NULL;
    }
    mraa_gpio_dir(dev->stepI2, MRAA_GPIO_OUT);

    if ( !(dev->stepI3 = mraa_gpio_init(i3)) )
    {
        printf("%s: mraa_gpio_init(i3) failed\n",
               __FUNCTION__);
        uln200xa_close(dev);
        return NULL;
    }
    mraa_gpio_dir(dev->stepI3, MRAA_GPIO_OUT);

    if ( !(dev->stepI4 = mraa_gpio_init(i4)) )
    {
        printf("%s: mraa_gpio_init(i4) failed\n",
               __FUNCTION__);
        uln200xa_close(dev);
        return NULL;
    }
    mraa_gpio_dir(dev->stepI4, MRAA_GPIO_OUT);

    // set default speed to 1
    uln200xa_set_speed(dev, 1);

    return dev;
}

void uln200xa_close(uln200xa_context dev)
{
    assert(dev != NULL);

    uln200xa_release(dev);
    if (dev->stepI1)
        mraa_gpio_close(dev->stepI1);
    if (dev->stepI2)
        mraa_gpio_close(dev->stepI2);
    if (dev->stepI3)
        mraa_gpio_close(dev->stepI3);
    if (dev->stepI4)
        mraa_gpio_close(dev->stepI4);

    free(dev);
}

void uln200xa_set_speed(const uln200xa_context dev, unsigned int speed)
{
    assert(dev != NULL);

    dev->stepDelay = 60 * 1000 / dev->stepsPerRev / speed;
}

void uln200xa_set_direction(const uln200xa_context dev,
                            ULN200XA_DIRECTION_T dir)
{
    assert(dev != NULL);

    switch (dir)
    {
    case ULN200XA_DIR_CW:
        dev->stepDirection = 1;
        break;
    case ULN200XA_DIR_CCW:
        dev->stepDirection = -1;
        break;
    }
}

static void uln200xa_stepper_step(const uln200xa_context dev)
{
    assert(dev != NULL);

    int step = dev->currentStep % 8;

    // This motor requires a different sequencing order in 8-steps than
    // usual.

    //   Step I0 I1 I2 I3
    //     1  0  0  0  1
    //     2  0  0  1  1
    //     3  0  0  1  0
    //     4  0  1  1  0
    //     5  0  1  0  0
    //     6  1  1  0  0
    //     7  1  0  0  0
    //     8  1  0  0  1

    switch (step)
    {
    case 0:    // 0001
        mraa_gpio_write(dev->stepI1, 0);
        mraa_gpio_write(dev->stepI2, 0);
        mraa_gpio_write(dev->stepI3, 0);
        mraa_gpio_write(dev->stepI4, 1);
        break;
    case 1:    // 0011
        mraa_gpio_write(dev->stepI1, 0);
        mraa_gpio_write(dev->stepI2, 0);
        mraa_gpio_write(dev->stepI3, 1);
        mraa_gpio_write(dev->stepI4, 1);
        break;
    case 2:    // 0010
        mraa_gpio_write(dev->stepI1, 0);
        mraa_gpio_write(dev->stepI2, 0);
        mraa_gpio_write(dev->stepI3, 1);
        mraa_gpio_write(dev->stepI4, 0);
        break;
    case 3:    // 0110
        mraa_gpio_write(dev->stepI1, 0);
        mraa_gpio_write(dev->stepI2, 1);
        mraa_gpio_write(dev->stepI3, 1);
        mraa_gpio_write(dev->stepI4, 0);
        break;
    case 4:    // 0100
        mraa_gpio_write(dev->stepI1, 0);
        mraa_gpio_write(dev->stepI2, 1);
        mraa_gpio_write(dev->stepI3, 0);
        mraa_gpio_write(dev->stepI4, 0);
        break;
    case 5:    // 1100
        mraa_gpio_write(dev->stepI1, 1);
        mraa_gpio_write(dev->stepI2, 1);
        mraa_gpio_write(dev->stepI3, 0);
        mraa_gpio_write(dev->stepI4, 0);
        break;
    case 6:    // 1000
        mraa_gpio_write(dev->stepI1, 1);
        mraa_gpio_write(dev->stepI2, 0);
        mraa_gpio_write(dev->stepI3, 0);
        mraa_gpio_write(dev->stepI4, 0);
        break;
    case 7:    // 1001
        mraa_gpio_write(dev->stepI1, 1);
        mraa_gpio_write(dev->stepI2, 0);
        mraa_gpio_write(dev->stepI3, 0);
        mraa_gpio_write(dev->stepI4, 1);
        break;
    default:
        // can't happen
        break;
    }
}

void uln200xa_stepper_steps(const uln200xa_context dev, unsigned int steps)
{
    assert(dev != NULL);

    while (steps > 0)
    {
        upm_delay_ms(dev->stepDelay);
        dev->currentStep += dev->stepDirection;

        if (dev->stepDirection == 1)
        {
            if (dev->currentStep >= dev->stepsPerRev)
                dev->currentStep = 0;
        }
        else
        {
            if (dev->currentStep <= 0)
                dev->currentStep = dev->stepsPerRev;
        }

        steps--;
        uln200xa_stepper_step(dev);
    }
}

void uln200xa_release(const uln200xa_context dev)
{
    assert(dev !=NULL);

    // we do these check since this is also called from
    // uln200xa_close() and we can't be sure that all of the contexts
    // have been created yet.
    if (dev->stepI1)
        mraa_gpio_write(dev->stepI1, 0);
    if (dev->stepI2)
        mraa_gpio_write(dev->stepI2, 0);
    if (dev->stepI3)
        mraa_gpio_write(dev->stepI3, 0);
    if (dev->stepI4)
        mraa_gpio_write(dev->stepI4, 0);
}
