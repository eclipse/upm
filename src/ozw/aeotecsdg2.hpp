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
   * @sensor dg2
   * @comname Aeotec Smart Dimmer Gen 2
   * @con uart
   * @type wifi
   * @man aeon
   * @web http://aeotec.com/z-wave-in-wall-switches/877-micro-sei-2e-manual-instructions.html
   *
   * @brief UPM API for Aeotec Smart Dimmer Gen 2
   *
   * This module allows for controlling and monitoring certain
   * elements of an Aeotec Smart Dimmer Gen 2.
   *
   * @snippet ozw-aeotecsdg2.cxx Interesting
   */

  class AeotecSDG2 : public ozwInterface {
  public:

    /**
     * These values correspond to the index values of the given node
     *
     */
    typedef enum : int {
      INDEX_Level                      = 0,
      INDEX_Energy_kWh                 = 3,
      INDEX_Power_Watts                = 6,
      INDEX_Volts                      = 9,
      INDEX_Current                    = 12
    } INDEX_VALUES_T;

    /**
     * AeotecSDG2 constructor
     *
     * @param nodeID The ZWave node number of the device we are
     * interested in.  Use the ozwdump example to see what nodes you
     * have available.
     */
    AeotecSDG2(int nodeID);

    /**
     * AeotecSDG2 Destructor
     */
    ~AeotecSDG2();

    /**
     * Set the output level of the dimmer switch.  This is a value
     * between 0 (fully off) and 99 (fully on).
     *
     * @param level The desired output level of the dimmer switch.
     * Valid values are between 0-99.
     */
    void setLevel(int level);

    /**
     * Return the current output level of the dimmer switch.  This
     * will be a number between 0 (fully off) and 99 (fully on).
     *
     * @return the current dimmer output level.
     */
    int getLevel();

    /**
     * Turn the switch fully on by setting the highest level (99).
     * This is equivalent to calling setLevel(99).
     */
    void on()
    {
      setLevel(99);
    }

    /**
     * Turn the switch off by setting the lowest level (0).  This is
     * equivalent to calling setLevel(0).
     */
    void off()
    {
      setLevel(0);
    }

    /**
     * Check to see if the switch is on or off.  Any dimmer level
     * greater that 0 is considered 'on'.
     *
     * @return true if the dimmer switch is on, false otherwise.
     */
    bool isOn();

    /**
     * Query the device and update internal state.  Strictly speaking,
     * this is not really neccessary -- a device may send this data on
     * it's own at periodic intervals.  This method causes OZW to
     * specifically query the device for it's current Energy, Watts,
     * Volts, Current, and dimmer Level status.  The device may take
     * some time to respond.
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
