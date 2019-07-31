/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015-2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include "wfs.h"

namespace upm {

/**
 * @brief Water Flow Sensor
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

 * @brief API for the Water Flow Sensor
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
         * Water Flow Sensor constructor
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
        /* Disable implicit copy and assignment operators */
        WFS(const WFS&) = delete;
        WFS &operator=(const WFS&) = delete;
};
}
