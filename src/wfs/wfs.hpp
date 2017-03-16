/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015-2017 Intel Corporation.
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

#include "wfs.h"

namespace upm {

/**
 * @brief Grove Water Flow Sensor library
 * @defgroup wfs libupm-wfs
 * @ingroup seeed gpio liquid eak
 */

/**
 * @library wfs
 * @sensor wfs
 * @comname Water Flow Sensor
 * @altname Grove Water Flow Sensor
 * @type liquid
 * @man seeed
 * @web http://wiki.seeedstudio.com/wiki/G1/2_Water_Flow_sensor
 * @con gpio
 * @kit eak

 * @brief API for the Grove Water Flow Sensor
 *
 * This sensor is used to measure water flow in liters per
 * minute (LPM). It incorporates a Hall Effect sensor. The UPM module
 * defines an interrupt routine to be triggered on each low pulse,
 * keeping count. This device requires a 10K pull-up resistor for
 * the signal line (yellow wire). There is a schematic diagram on
 * the SeeedStudio site (3/2015):
 * http://www.seeedstudio.com/wiki/index.php?title=G1/2_Water_Flow_sensor
 *
 * However, be careful when wiring this up - the schematic appears to
 * have a bug in it: the lower left connection of the signal line
 * (yellow) to Vcc (red) should not be there. The sensor can work
 * with this connection, but probably not for very long.
 *
 * @image html wfs.jpg
 * @snippet wfs.cxx Interesting
 */
class WFS {
    public:
        /**
            * Grove Water Flow sensor constructor
            *
            * @param pin Digital pin to use
            */
        WFS(int pin);
        /**
            * WFS destructor
            */
        ~WFS();

        /**
            * Returns the number of milliseconds elapsed since initClock()
            * was last called.
            *
            * @return Elapsed milliseconds
            */
        uint32_t getMillis();

        /**
            * Resets the clock
            *
            */
        void initClock();

        /**
            * Resets the flow counter to 0. The flow counter should be
            * stopped via stopFlowCounter() prior to calling this function.
            *
            */
        void clearFlowCounter() { wfs_clear_flow_counter(m_wfs); };

        /**
            * Starts the flow counter
            *
            */
        void startFlowCounter();

        /**
            * Stops the flow counter
            *
            */
        void stopFlowCounter();

        /**
            * Gets the flow counter
            *
            * @return Flow counter
            */
        uint32_t flowCounter() { return wfs_flow_counter(m_wfs); };

        /**
            * Computes the flow rate in liters per minute (LPM).  Note, this
            * is for the Grove WFS.  If you are using some other WFS, you
            * should compute the flow rate on your own based on the data for
            * your sensor.
            *
            * @return Computed flow rate
            */
        float flowRate();

    protected:
        wfs_context m_wfs;

    private:
};
}
