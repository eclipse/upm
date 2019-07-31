/*
 * Author: Jun Kato <i@junkato.jp>
 * Copyright (c) 2015 Jun Kato.
 *
 * Thanks to Seeed Studio for a working arduino sketch
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <functional>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

#include "ultrasonic.hpp"

using namespace upm;

UltraSonic::UltraSonic(int pin)
{
    m_name = "UltraSonic";

    mraa_init();

    // setup pin
    m_pinCtx = mraa_gpio_init(pin);
    if (m_pinCtx == NULL) {
        fprintf(stderr, "Are you sure that pin%d you requested is valid on your platform?", pin);
        exit(1);
    }
    mraa_gpio_isr (m_pinCtx, MRAA_GPIO_EDGE_BOTH,
                   &signalISR, this);
}

UltraSonic::~UltraSonic()
{
    // close pin
    mraa_gpio_isr_exit(m_pinCtx);
    mraa_gpio_close(m_pinCtx);
}

float
UltraSonic::getDistance()
{
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
UltraSonic::signalISR(void* ctx)
{
    upm::UltraSonic* This = (upm::UltraSonic*) ctx;
    This->ackEdgeDetected();
}

void
UltraSonic::ackEdgeDetected()
{
    if (++m_InterruptCounter % 2 == 0) {
        gettimeofday(&m_FallingTimeStamp, NULL);
        m_doWork = false;
    } else {
        gettimeofday(&m_RisingTimeStamp, NULL);
    }
}
