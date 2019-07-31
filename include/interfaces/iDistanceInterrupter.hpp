/*
 * Author: Mihai Stefanescu <mihai.stefanescu@rinftech.com>
 * Copyright (c) 2018 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

namespace upm
{
/**
 * @brief Interface for Distance Interrupter Sensors
*/
  class iDistanceInterrupter
  {
  public:
    virtual ~iDistanceInterrupter() {}

    /**
     * Determines if objected is detected
     * 
     * @return True if the sensor has detected an object
     */
    virtual bool objectDetected() = 0;
  };
}
