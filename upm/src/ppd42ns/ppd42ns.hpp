/*
 * Author: Zion Orent <sorent@ics.com>
 *         Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014-2016 Intel Corporation.
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


