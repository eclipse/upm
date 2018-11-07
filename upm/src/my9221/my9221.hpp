/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * These modules were rewritten, based on original work by:
 *
 * (original my9221/groveledbar)
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * (grovecircularled)
 * Author: Jun Kato and Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Contributions: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
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
#include <my9221.h>

namespace upm {

    /**
     * @brief MY9221 LED Controller Library
     * @defgroup my9221 libupm-my9221
     * @ingroup seeed display gpio eak
     */
    class MY9221 {
    public:

        /**
         * Instantiates an MY9221 object
         *
         * @param dataPin Data pin
         * @param clockPin Clock pin
         * @param instances Number of daisy-chained my9221s, default 1
         */
        MY9221(int dataPin, int clockPin, int instances=1);

        /**
         * MY9221 destructor
         */
        virtual ~MY9221();

        /**
         * Enable or disable auto refresh.  When auto refresh is enabled,
         * update the LED display as soon as the internal state changes.
         * When false, the display(s) will not be updated until the
         * refresh() method is called.
         *
         * @param enable true to enable auto refresh, false otherwise
         */
        void setAutoRefresh(bool enable)
        {
            my9221_set_auto_refresh(m_my9221, enable);
        }

        /**
         * Set an LED to a specific on (high intensity) or off (low
         * intensity) value.
         *
         * @param led The LED whose state you wish to change
         * @param on true to turn on the LED, false to turn the LED off
         */
        void setLED(int led, bool on);

        /**
         * Set the greyscale intensity of an LED in the OFF state.  The
         * intensity is a value from 0 (fully off) to 255 (fully on).
         * This will take effect on any future LED set or clear
         * operations.
         *
         * @param intensity a value from 0 (fully off) to 255 (fully on)
         */
        void setLowIntensityValue(int intensity);

        /**
         * Set the greyscale intensity of an LED in the ON state.  The
         * intensity is a value from 0 (fully off) to 255 (fully on).
         * This will take effect on any future LED set or clear
         * operations.
         *
         * @param intensity a value from 0 (fully off) to 255 (fully on)
         */
        void setHighIntensityValue(int intensity);

        /**
         * Set all of the LEDS to the ON (high intensity value) state.
         */
        void setAll();

        /**
         * Set all of the LEDS to the OFF (low intensity value) state.
         */
        void clearAll();

        /**
         * Set the LED states to match the internal stored states.  This
         * is useful when auto refresh (setAutoRefresh()) is false to
         * update the display.
         */
        void refresh();

    protected:

        my9221_context m_my9221;

    private:
        /* Disable implicit copy and assignment operators */
        MY9221(const MY9221&) = delete;
        MY9221 &operator=(const MY9221&) = delete;
    };

}
