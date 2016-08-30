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
   * @comname Aeotec Smart Dimmer Gen 2
   * @con uart
   *
   * @brief UPM API for Aeotec Smart Dimmer Gen 2
   *
   * This module allows for controlling and monitoring certain
   * elements of an Aeotec Smart Dimmer Gen 2.
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
