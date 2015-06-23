/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
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
#pragma once

#include <string>
#include <mraa/gpio.h>

namespace upm {

/**
 * @brief RPR220 IR reflective sensor library
 * @defgroup rpr220 libupm-rpr220
 * @ingroup seeed gpio light
 */
/**
 * @library rpr220
 * @sensor rpr220
 * @comname RPR220 IR Reflective Sensor
 * @altname Grove IR Reflective Sensor
 * @type light
 * @man seeed
 * @web http://www.seeedstudio.com/wiki/Grove_-_Infrared_Reflective_Sensor
 * @con gpio
 * @kit tsk
 *
 * @brief API for the RPR220 based Grove IR Reflective Sensor
 * 
 * UPM module for the Grove IR Reflective Sensor.  The sensitivity
 * can be adjusted with the potentiometer on the sensor module.  It
 * has a range of approximately 15mm, and a fast response time.
 *
 * It detects high contrast dark areas on a light background.
 *
 * This module allows the user to determine the current status
 * (black detected or not).  Additionally, if desired, an Interrupt
 * Service Routine (ISR) can be installed that will be called when
 * black is detected.  Either method can be used, depending on your
 * use case.
 *
 * @image html rpr220.jpg
 * @snippet rpr220.cxx Interesting
 * @snippet rpr220-intr.cxx Interesting
 */
  class RPR220 {
  public:
    /**
     * Grove IR Reflective Sensor constructor
     *
     * @param pin digital pin to use
     */
    RPR220(int pin);

    /**
     * RPR220 Destructor
     */
    ~RPR220();

    /**
     * Get the status of the pin, true means black detected
     *
     * @return true if the sensor is detecting black
     */
    bool blackDetected();

    /**
     * Install an Interrupt Service Routine (ISR) to be called when
     * black is detected
     *
     * @param fptr function pointer to function to be called on interrupt
     * @param arg pointer to an object that will be supplied as an
     * arguement to the ISR.
     */
    void installISR(void (*isr)(void *), void *arg);

    /**
     * Uninstall the previously installed Interrupt Service Routine (ISR)
     *
     */
    void uninstallISR();

  private:
    bool m_isrInstalled;
    mraa_gpio_context m_gpio;
  };
}


