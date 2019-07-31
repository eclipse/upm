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
   * @sensor DSB29
   * @comname Aeotec Door/Window Sensor 2nd Edition
   * @altname DSB29
   * @type wifi
   * @con uart
   * @man aeon
   * @web http://aeotec.com/z-wave-door-window-sensor
   *
   * @brief UPM API for Aeotec Door/Window Sensor 2nd Edition
   *
   * This module allows for monitoring certain elements of an Aeotec
   * Door/Window Sensor 2nd Edition Z-Wave device.
   *
   * NOTE: This is a battery powered device that spends most of it's
   * time sleeping (sort of like a cat). This means that on initial
   * startup, there is not enough information known about the device
   * to reliably query anything.  Use isDeviceAvailable() to test
   * whether the device has responded to OZW's probe request before
   * requesting information for it.
   *
   * The device information should become known once the device has
   * awakened, either via it's configuration, or manually via the
   * include button on the device.  By default the device will never
   * wake on it's own, so you should use the openzwave control panel
   * or similar software to configure a periodic wakeup time.
   *
   * @snippet ozw-aeotecdw2e.cxx Interesting
   */

  class AeotecDW2E : public ozwInterface {
  public:

    /**
     * These values correspond to the index values of the given node
     *
     */
    typedef enum : int {
      INDEX_Alarm                      = 0, // alarm
      INDEX_AlarmLevel                 = 2, // tamper switch
      INDEX_BatteryLevel               = 3
    } INDEX_VALUES_T;

    /**
     * AeotecDW2E constructor
     *
     * @param nodeID The ZWave node number of the device we are
     * interested in.  Use the ozwdump example to see what nodes you
     * have available.
     */
    AeotecDW2E(int nodeID);

    /**
     * AeotecDW2E Destructor
     */
    ~AeotecDW2E();

    /**
     * Returns true if the node (device) information has been
     * received yet, false otherwise.  A sleeping node (like this
     * device) will not be available for monitoring until all node
     * info has been received.  This will occur once the device has
     * been awakened and has responded to OZW's probe request.
     *
     * @return true if the node is available, false otherwise.
     */
    bool isDeviceAvailable();

    /**
     * Return the alarm value.  If the device is not available yet
     * (see isDeviceAvailable()), false will always be returned.
     *
     * @return true if in the tripped state, false otherwise.
     */
    bool isAlarmTripped();

    /**
     * Return the alarm's tamper switch valu.  If the device is not
     * available yet (see isDeviceAvailable()), false will always be
     * returned.
     *
     * @return true if in the tripped state, false otherwise.
     */
    bool isTamperTripped();

    /**
     * Return the current bettery level of the sensor as a percentage.
     * The number returned will be between 0-100.  If the device is
     * not available yet (see isDeviceAvailable()), 0 will always be
     * returned.
     *
     * @return The Battery power level.
     */
    int getBatteryLevel();


  protected:
  private:
  };
}
