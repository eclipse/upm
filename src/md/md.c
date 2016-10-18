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

#include <unistd.h>
#include <assert.h>
#include <string.h>

#include <upm_utilities.h>

#include "md.h"

// static (private) functions
static void md_stepper_step(const md_context dev)
{
    assert(dev != NULL);

    int step = dev->currentStep % 4;

    switch (step)
    {
    case 0:
        md_write_packet(dev, MD_REG_SET_DIRECTION, 0b0101, MD_NOOP);
        break;
    case 1:
        md_write_packet(dev, MD_REG_SET_DIRECTION, 0b0110, MD_NOOP);
        break;
    case 2:
        md_write_packet(dev, MD_REG_SET_DIRECTION, 0b1010, MD_NOOP);
        break;
    case 3:
        md_write_packet(dev, MD_REG_SET_DIRECTION, 0b1001, MD_NOOP);
        break;
    default:
        // can't happen in this universe
        break;
    }
}

md_context md_init(int bus, uint8_t address)
{
    md_context dev =
        (md_context)malloc(sizeof(struct _md_context));

    if (!dev)
        return NULL;

    memset((void *)dev, 0, sizeof(struct _md_context));
    dev->i2c = NULL;
    dev->initialized = false;

    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        md_close(dev);
        return NULL;
    }

    // MRAA contexts...
    if ( !(dev->i2c = mraa_i2c_init(bus)) )
    {
        printf("%s: mraa_gpio_init() failed\n",
               __FUNCTION__);
        md_close(dev);
        return NULL;
    }

    // this board *requires* 100Khz i2c bus only
    if (mraa_i2c_frequency(dev->i2c, MRAA_I2C_STD))
    {
        printf("%s: mraa_i2c_frequency(MRAA_I2C_STD) failed\n", __FUNCTION__);
        md_close(dev);
        return NULL;
    }

    if (mraa_i2c_address(dev->i2c, address))
    {
        printf("%s: mraa_i2c_frequency(MRAA_I2C_STD) failed\n", __FUNCTION__);
        md_close(dev);
        return NULL;
    }

    // default to mode1 stepper operation, 200 steps per rev.
    md_config_stepper(dev, 200, MD_STEP_MODE1);

    dev->initialized = true;

    return dev;
}

void md_close(md_context dev)
{
    assert(dev != NULL);

    if (dev->initialized)
    {
        md_set_motor_speeds(dev, 0, 0);
        md_write_packet(dev, MD_REG_SET_DIRECTION, 0, MD_NOOP);
    }

    if (dev->i2c)
        mraa_i2c_stop(dev->i2c);

    free(dev);
}

bool md_write_packet(const md_context dev, MD_REG_T reg,
                     uint8_t data1, uint8_t data2)
{
    assert(dev != NULL);

    uint8_t buf[3];

    buf[0] = reg;
    buf[1] = data1;
    buf[2] = data2;

    if (mraa_i2c_write(dev->i2c, buf, 3))
    {
        printf("%s: mraa_i2c_write() failed\n", __FUNCTION__);
        return false;
    }

    // This sleep appears to be required.  Without it, writes randomly
    // fail (no ACK received).  This happens most often on the SET_SPEED
    // packet.  I am guessing that there is a timing problem and/or bug
    // in the motor driver's firmware.

    upm_delay_us(100);

    return true;
}

bool md_set_motor_speeds(const md_context dev, uint8_t speedA, uint8_t speedB)
{
    assert(dev != NULL);

    return md_write_packet(dev, MD_REG_SET_SPEED, speedA, speedB);
}

bool md_set_pwm_frequency_prescale(const md_context dev, uint8_t freq)
{
    assert(dev != NULL);

    return md_write_packet(dev, MD_REG_SET_PWM_FREQ, freq, MD_NOOP);
}

bool md_set_motor_directions(const md_context dev, MD_DC_DIRECTION_T dirA,
                             MD_DC_DIRECTION_T dirB)
{
    assert(dev != NULL);

    uint8_t dir = ((dirB & 0x03) << 2) | (dirA & 0x03);
    return md_write_packet(dev, MD_REG_SET_DIRECTION, dir, MD_NOOP);
}

bool md_enable_stepper(const md_context dev, MD_STEP_DIRECTION_T dir,
                       uint8_t speed)
{
    assert(dev != NULL);

    // If mode 2, send the command and return immediately
    if (dev->stepMode == MD_STEP_MODE2)
        return md_write_packet(dev, MD_REG_STEPPER_ENABLE, dir, speed);

    // otherwise, mode 1, setup the basics and start stepping.

    dev->stepDelay = 60 * 1000 / dev->stepsPerRev / speed;
    dev->stepDirection = ((dir == MD_STEP_DIR_CW) ? 1 : -1);

    // seeed says speed should always be 255,255 for stepper operation
    md_set_motor_speeds(dev, 255, 255);

    while (dev->totalSteps > 0)
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

        dev->totalSteps--;
        md_stepper_step(dev);
    }

    // and... we're done
    return true;
}

bool md_disable_stepper(const md_context dev)
{
    assert(dev != NULL);

    if (dev->stepMode == MD_STEP_MODE2)
        return md_write_packet(dev, MD_REG_STEPPER_DISABLE, MD_NOOP, MD_NOOP);

    // else, mode 1
    md_write_packet(dev, MD_REG_SET_DIRECTION, 0, MD_NOOP);
    return md_set_motor_speeds(dev, 0, 0);
}

bool md_set_stepper_steps(const md_context dev, unsigned int steps)
{
    assert(dev != NULL);

    if (dev->stepMode == MD_STEP_MODE2)
    {
        if (steps == 0)
        {
            // invalid
            printf("%s: invalid number of steps. "
                   "Valid values are between 1 and 255. \n", __FUNCTION__);
            return false;
        }
        return md_write_packet(dev, MD_REG_STEPPER_NUM_STEPS, steps, MD_NOOP);
    }

    // for mode one, just store it for future use by enableStepper()
    dev->totalSteps = steps;
    return true;
}

void md_config_stepper(const md_context dev, unsigned int stepsPerRev,
                       MD_STEP_MODE_T mode)
{
    assert(dev != NULL);

    dev->stepsPerRev = stepsPerRev;
    dev->stepMode = mode;
    dev->currentStep = 0;
    dev->stepDelay = 0;
    dev->stepDirection = 1;
    dev->totalSteps = 0;
}
