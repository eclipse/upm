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
   * @comname Aeotec Door/Window Sensor 2nd Edition
   * @altname DSB29
   * @con uart
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
