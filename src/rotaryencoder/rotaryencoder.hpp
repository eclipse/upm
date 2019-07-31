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

#include "rotaryencoder.h"
#include <interfaces/iAngle.hpp>

namespace upm {

    /**
     * @brief Rotary Encoder
     * @defgroup rotaryencoder libupm-rotaryencoder
     * @ingroup seeed gpio other hak
     */

    /**
     * @library rotaryencoder
     * @sensor rotaryencoder
     * @comname Rotary Encoder
     * @type other
     * @man seeed
     * @web http://www.seeedstudio.com/wiki/Grove_-_Encoder
     * @con gpio
     * @kit hak
     *
     * @brief API for the Grove Rotary Encoder
     *
     * UPM module for the Grove rotary encoder. This rotary encoder
     * encodes a rotation signal into electronic pulses that can be used
     * to measure rotation and direction. It is useful in cases where a
     * rotary knob is required, but using a potentiometer is not
     * desirable. A rotary encoder can turn a full 360 degrees
     * without a stop and does not place a resistive load on the
     * circuit, as is the case with a potentiometer.
     *
     * This module maintains a position that is incremented or
     * decremented according to the rotation on the encoder.
     *
     * @image html rotaryencoder.jpg
     * @snippet rotaryencoder.cxx Interesting
     */

    class RotaryEncoder : virtual public iAngle {
    public:
        /**
         * RotaryEncoder constructor
         *
         * @param pinA Digital pin to use for signal A
         * @param pinB Digital pin to use for signal B
         */
        RotaryEncoder(int pinA, int pinB);
        /**
         * RotaryEncoder destructor
         */
        ~RotaryEncoder();

        /**
         * Resets the position to a given number; default is 0.
         *
         * @param count Integer to initialize the position to
         */
        void initPosition(int count=0);

        /**
         * Gets the position value
         *
         */
        int position();

        /**
         * Get rotation value from sensor data.
         *
         * @return rotation value.
         */
        virtual float getAngle();

    private:
        /* Disable implicit copy and assignment operators */
        RotaryEncoder(const RotaryEncoder&) = delete;
        RotaryEncoder &operator=(const RotaryEncoder&) = delete;

        rotaryencoder_context m_rotaryencoder;
    };
}
