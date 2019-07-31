/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015-2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>
#include <rpr220.h>

namespace upm {

    /**
     * @brief RPR220 IR Reflective Sensor
     * @defgroup rpr220 libupm-rpr220
     * @ingroup seeed gpio light tsk hak
     */
    /**
     * @library rpr220
     * @sensor rpr220
     * @comname Infrared (IR) Reflective Sensor
     * @altname Grove IR Reflective Sensor
     * @type light
     * @man seeed
     * @web http://www.seeedstudio.com/wiki/Grove_-_Infrared_Reflective_Sensor
     * @con gpio
     * @kit tsk hak
     *
     * @brief API for the RPR220-based Grove IR Reflective Sensor
     *
     * UPM module for the Grove IR reflective sensor. The sensitivity
     * can be adjusted with the potentiometer on the sensor module. It
     * has a range of approximately 15 mm, and a quick response time.
     *
     * It detects high-contrast dark areas on a light background.
     *
     * This module allows the user to determine the current status
     * (black detected or not). Additionally, if desired, an interrupt
     * service routine (ISR) can be installed that is called when
     * black is detected. Either method can be used, depending on your
     * use case.
     *
     * @image html rpr220.jpg
     * @snippet rpr220.cxx Interesting
     * @snippet rpr220-intr.cxx Interesting
     */
    class RPR220 {
    public:
        /**
         * RPR220 constructor
         *
         * @param pin Digital pin to use
         */
        RPR220(int pin);

        /**
         * RPR220 destructor
         */
        ~RPR220();

        /**
         * Gets the status of the pin; true means black has been detected
         *
         * @return True if the sensor has detected black
         */
        bool blackDetected();


        /**
         * Installs an ISR to be called when
         * black is detected
         *
         * @param isr Pointer to a function to be called on interrupt
         * @param arg Pointer to an object to be supplied as an
         * argument to the ISR.
         */
        void installISR(void (*isr)(void *), void *arg);

        /**
         * Uninstalls the previously installed ISR
         *
         */
        void uninstallISR();

    private:
        /* Disable implicit copy and assignment operators */
        RPR220(const RPR220&) = delete;
        RPR220 &operator=(const RPR220&) = delete;

        rpr220_context m_rpr220;

    };
}
