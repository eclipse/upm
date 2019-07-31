/*
 * Author: Stefan Andritoiu <stefan.andritoiu@gmail.com>
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
 * @brief Interface for Water sensors
*/
  class iWater
  {
  public:
    virtual ~iWater() {}

    /**
     * Gets the water (wet/not wet) value from the sensor
     *
     * @return True if the sensor is wet, false otherwise
     */
    virtual bool isWet() = 0;

  };
}
