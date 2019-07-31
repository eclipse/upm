/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014-2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <linefinder.h>
#include <interfaces/iLineFinder.hpp>

namespace upm {
    /**
     * @brief Line Finder Sensor
     * @defgroup linefinder libupm-linefinder
     * @ingroup seeed dfrobot gpio color robok
     */

    /**
     * @library linefinder
     * @sensor linefinder
     * @comname Infrared (IR) Based Line Finder
     * @altname Grove Line Finder, DFRobot Line Tracking sensor
     * @type color
     * @man seeed dfrobot
     * @con gpio
     * @kit robok
     *
     * @brief API for the  Line Finder Sensor
     *
     * UPM module for the Line Finder sensor. It outputs a
     * digital signal indicating whether it is detecting black on a
     * white background, or white on a black background.
     *
     * @image html linefinder.jpg
     * @snippet linefinder.cxx Interesting
     */
    class LineFinder : virtual public iLineFinder {
    public:
        /**
         *  Line Finder digital sensor constructor
         *
         * @param pin Digital pin to use
         */
        LineFinder(int pin);

        /**
         * LineFinder destructor
         */
        ~LineFinder();

        /**
         * Determines whether white has been detected
         *
         * @return True if white is detected
         */
        virtual bool whiteDetected();

        /**
         * Determines whether black has been detected
         *
         * @return True if black is detected
         */
        virtual bool blackDetected();

    private:
        /* Disable implicit copy and assignment operators */
        LineFinder(const LineFinder&) = delete;
        LineFinder &operator=(const LineFinder&) = delete;

        linefinder_context m_linefinder;
    };
}
