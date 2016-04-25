/*
 * Author: Jun Kato <i@junkato.jp>
 * Copyright (c) 2015 Jun Kato.
 *
 * Thanks to Seeed Studio for a working arduino sketch
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

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <functional>

#include "groveultrasonic.hpp"

using namespace upm;

GroveUltraSonic::GroveUltraSonic (uint8_t pin) {
    mraa_result_t error = MRAA_SUCCESS;
    m_name = "GroveUltraSonic";

    mraa_init();

    // setup pin
    m_pinCtx = mraa_gpio_init(pin);
    if (m_pinCtx == NULL) {
        fprintf (stderr, "Are you sure that pin%d you requested is valid on your platform?", pin);
        exit (1);
    }
    mraa_gpio_use_mmaped(m_pinCtx, 1);
    mraa_gpio_isr (m_pinCtx, MRAA_GPIO_EDGE_BOTH,
                   &signalISR, this);
}

GroveUltraSonic::~GroveUltraSonic () {

    // close pin
    mraa_gpio_isr_exit(m_pinCtx);
    mraa_gpio_close (m_pinCtx);
}

int
GroveUltraSonic::getDistance () {

    // output trigger signal
    mraa_gpio_dir(m_pinCtx, MRAA_GPIO_OUT);
    mraa_gpio_write(m_pinCtx, LOW);
    usleep(2);
    mraa_gpio_write(m_pinCtx, HIGH);
    usleep(5);
    mraa_gpio_write(m_pinCtx, LOW);

    // wait for the pulse,
    m_doWork = true;
    m_InterruptCounter = 0;
    mraa_gpio_dir(m_pinCtx, MRAA_GPIO_IN);

    // though do not wait over 25 [ms].
    int timer = 0;
    while (m_doWork && timer++ < 5) {
        // in 25 [ms], sound travels 25000 / 29 / 2 = 431 [cm],
        // which is more than 400 [cm], the max distance measurable with this sensor.
        usleep(5 * 1000); // 5 [ms]
    }

    // calc diff
    long diff = m_FallingTimeStamp.tv_usec - m_RisingTimeStamp.tv_usec;
    diff += (m_FallingTimeStamp.tv_sec - m_RisingTimeStamp.tv_sec) * 1000000;
    return timer >= 5 ? 0 : diff;
}

void
GroveUltraSonic::signalISR(void *ctx) {
    upm::GroveUltraSonic *This = (upm::GroveUltraSonic *)ctx;
    This->ackEdgeDetected();
}

void
GroveUltraSonic::ackEdgeDetected () {
    if (++m_InterruptCounter % 2 == 0) {
        gettimeofday(&m_FallingTimeStamp, NULL);
        m_doWork = false;
    } else {
        gettimeofday(&m_RisingTimeStamp, NULL);
    }
}
