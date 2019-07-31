/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>

#include "ozwinterface.hpp"

namespace upm {

  /**
   * @library ozw
   * @sensor ss6
   * @comname Aeotec Smart Switch 6
   * @con uart
   * @type wifi
   * @man aeon
   * @web http://aeotec.com/z-wave-plug-in-switch
   *
   * @brief UPM API for Aeotec Smart Switch 6
   *
   * This module allows for controlling and monitoring certain
   * elements of an Aeotec Smart Switch 6.
   *
   * @snippet ozw-aeotecss6.cxx Interesting
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
