/*
 * Author: Zion Orent <sorent@ics.com>
 *         Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014-2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>
#include <ppd42ns.h>

namespace upm {

    /**
     * @brief PPD42NS Dust Sensor
     * @defgroup ppd42ns libupm-ppd42ns
     * @ingroup seeed gpio other eak
     */
    /**
     * @library ppd42ns
     * @sensor ppd42ns
     * @comname Dust Sensor
     * @altname Grove Dust Sensor
     * @type other
     * @man seeed
     * @web http://www.seeedstudio.com/wiki/Grove_-_Dust_Sensor
     * @con gpio
     * @kit eak
     *
     * @brief API for the PPD42NS Dust Sensor
     *
     * UPM module for the PPD42NS dust sensor
     *
     * @image html ppd42ns.jpg
     * @snippet ppd42ns.cxx Interesting
     */
    class PPD42NS {
    public:

        /**
         * PPD42NS constructor
         *
         * @param pin Digital pin to use
         */
        PPD42NS(int pin);

        /**
         * PPD42NS destructor
         */
        ~PPD42NS();

        /**
         * Prints dust concentration
         *
         * @return struct ppd42ns_dust_data Contains data from the dust sensor
         */
        ppd42ns_dust_data getData();

    private:
        /* Disable implicit copy and assignment operators */
        PPD42NS(const PPD42NS&) = delete;
        PPD42NS &operator=(const PPD42NS&) = delete;

        ppd42ns_context m_ppd42ns;
    };
}


