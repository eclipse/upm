/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
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

#include "ozwinterface.hpp"

namespace upm {

  /**
   * @library ozw
   * @comname Aeotec Smart Switch 6
   * @con uart
   *
   * @brief UPM API for Aeotec Smart Switch 6
   *
   * This module allows for controlling and monitoring certain
   * elements of an Aeotec Smart Switch 6.
   */

  class AeotecSS6 : public ozwInterface {
  public:

    /**
     * These values correspond to the index values of the given node
     *
     */
    typedef enum : int {
      INDEX_SwitchCtrl                 = 0,
      INDEX_Energy_kWh                 = 4,
      INDEX_Power_Watts                = 7,
      INDEX_Volts                      = 8,
      INDEX_Current                    = 9
    } INDEX_VALUES_T;

    /**
     * AeotecSS6 constructor
     *
     * @param nodeID The ZWave node number of the device we are
     * interested in.  Use the ozwdump example to see what nodes you
     * have available.
     */
    AeotecSS6(int nodeID);

    /**
     * AeotecSS6 Destructor
     */
    ~AeotecSS6();

    /**
     * Turn the switch on.
     */
    void on();

    /**
     * Turn the switch off.
     */
    void off();

    /**
     * Check to see if the switch is on or off.
     *
     * @return true if the switch is on, false otherwise.
     */
    bool isOn();

    /**
     * Query the device and update internal state.  Strictly speaking,
     * this is not really neccessary -- a device may send this data on
     * it's own at periodic intervals.  This method causes OZW to
     * specifically query the device for it's current Energy, Watts,
     * Volts, Current, and Switch status.  The device may take some
     * time to respond.
     */
    void update();

    /**
     * Return the device's accumulated energy usage in kilo-watt-hours
     * (kWh).
     *
     * @return The energy usage in kWh.
     */
    float getEnergy();

    /**
     * Return the device's current power consumption in Watts.
     *
     * @return The current power consumption in Watts.
     */
    float getWatts();

    /**
     * Return the device's current voltage measurement in Volts.
     *
     * @return The volts measurement.
     */
    float getVolts();

    /**
     * Return the device's current consumption in Amps.
     *
     * @return The current consumption measurement in Amps.
     */
    float getCurrent();


  protected:
  private:
  };
}
