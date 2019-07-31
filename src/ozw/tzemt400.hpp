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
   * @sensor tzemt400
   * @comname Trane TZEMT400 Z-Wave Thermostat.
   * @con uart
   * @man trane
   *
   * @brief UPM API for Trane TZEMT400 Z-Wave Thermostat
   *
   * This module allows for monitoring certain elements of a Trane
   * TZEMT400 Z-Wave Thermostat.  It was tested with a Trane
   * TZEMT400BB32MAA variant.
   *
   * @snippet ozw-tzemt400.cxx Interesting
   */

  class TZEMT400 : public ozwInterface {
  public:

    /**
     * These values correspond to the index values of the given node
     *
     */
    typedef enum : int {
      INDEX_Temperature                = 0,
      INDEX_Mode                       = 1,
      INDEX_Operating_State            = 2,
      INDEX_Heating_Point              = 3,
      INDEX_Cooling_Point              = 4,
      INDEX_Fan_Mode                   = 5,
      INDEX_Fan_State                  = 6,
      INDEX_FC_Units                   = 14 // F or C units
    } INDEX_VALUES_T;

    /**
     * TZEMT400 constructor
     *
     * @param nodeID The ZWave node number of the device we are
     * interested in.  Use the ozwdump example to see what nodes you
     * have available.
     */
    TZEMT400(int nodeID);

    /**
     * TZEMT400 Destructor
     */
    ~TZEMT400();

    /**
     * Return the current measured temperature in Celsius or
     * Fahrenheit.
     *
     * @param fahrenheit true to return data in Fahrenheit, false for
     * Celicus.  Celsius is the default.
     * @return The temperature in degrees Celsius or Fahrenheit.
     */
    float getTemperature(bool fahrenheit=false);

    /**
     * Return a string representing the current Mode.  This is usually
     * something like "Cool", "Heat", "Auto", etc.
     *
     * @return a string representing the current mode.
     */
    std::string getMode();

    /**
     * Return a string representing the current Operating State.  This
     * is usually something like "Idle", or "Cooling".
     *
     * @return a string representing the current operating state.
     */
    std::string getOperatingState();

    /**
     * Return the current Heating Point temperature in Celsius or
     * Fahrenheit.  This is the temperature at which the thermostat
     * will want to engage Heat.
     *
     * @param fahrenheit true to return data in Fahrenheit, false for
     * Celicus.  Celsius is the default.
     * @return The Heating Point temperature in degrees Celsius or
     * Fahrenheit.
     */
    float getHeatingPointTemperature(bool fahrenheit=false);

    /**
     * Return the current Cooling Point temperature in Celsius or
     * Fahrenheit.  This is the temperature at which the thermostat
     * will want to engage Cooling.
     *
     * @param fahrenheit true to return data in Fahrenheit, false for
     * Celicus.  Celsius is the default.
     * @return The Cooling Point temperature in degrees Celsius or
     * Fahrenheit.
     */
    float getCoolingPointTemperature(bool fahrenheit=false);

    /**
     * Return a string representing the current Fan Mode.  This
     * is usually something like "Auto Low".
     *
     * @return a string representing the Fan Mode.
     */
    std::string getFanMode();

    /*
     * Return a string representing the current Fan State.  This
     * is usually something like "Idle".
     *
     * @return a string representing the Fan State.
     */
    std::string getFanState();

    /**
     * Query the device and update internal state.  Strictly speaking,
     * this is not really neccessary -- a device may send this data on
     * it's own at periodic intervals.  This method causes OZW to
     * specifically query the device for it's current information.
     * The device may take some time to respond.
     */
    void update();


  protected:
  private:
  };
}
