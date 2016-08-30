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
   * @comname Aeotec Home Energy Meter Gen 1
   * @con uart
   *
   * @brief UPM API for Aeotec Home Energy Meter Gen 1
   *
   * This module allows for monitoring certain elements of an Aeotec
   * Home Energy Meter Gen 1.
   */

  class AeotecDSB09104 : public ozwInterface {
  public:

    /**
     * These values correspond to the index values of the given node
     *
     */
    typedef enum : int {
      INDEX_PowerC1                    = 0,
      INDEX_PowerC2                    = 1,
      INDEX_PowerC3                    = 2,
      INDEX_EnergyC1                   = 3,
      INDEX_EnergyC2                   = 4,
      INDEX_EnergyC3                   = 5,
      INDEX_BatteryLevel               = 18
    } INDEX_VALUES_T;

    /**
     * AeotecDSB09104 constructor
     *
     * @param nodeID The ZWave node number of the device we are
     * interested in.  Use the ozwdump example to see what nodes you
     * have available.
     */
    AeotecDSB09104(int nodeID);

    /**
     * AeotecDSB09104 Destructor
     */
    ~AeotecDSB09104();

    /**
     * Get the current Watts reading from Channel 1.
     *
     * @return The Watts reading.
     */
    float getWattsC1();

    /**
     * Get the current Watts reading from Channel 2.
     *
     * @return The Watts reading.
     */
    float getWattsC2();

    /**
     * Get the current Watts reading from Channel 3.
     *
     * @return The Watts reading.
     */
    float getWattsC3();

    /**
     * Get the current Energy reading from Channel 1.  The value is in
     * kWh.
     *
     * @return The Energy reading.
     */
    float getEnergyC1();

    /**
     * Get the current Energy reading from Channel 2.  The value is in
     * kWh.
     *
     * @return The Energy reading.
     */
    float getEnergyC2();

    /**
     * Get the current Energy reading from Channel 3.  The value is in
     * kWh.
     *
     * @return The Energy reading.
     */
    float getEnergyC3();

    /**
     * Query the device and update internal state.  Strictly speaking,
     * this is not really neccessary -- a device may send this data on
     * it's own at periodic intervals if configured to do so.  This
     * method causes OZW to specifically query the device for it's
     * current Energy, Watts, and Battery Level status.  The device
     * may take some time to respond.
     */
    void update();

    /**
     * Return the current bettery level of the meter as a percentage.
     * The number returned will be between 0-100.
     *
     * @return The Battery power level.
     */
    int getBatteryLevel();


  protected:
  private:
  };
}
