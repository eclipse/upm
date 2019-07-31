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
 * @brief Interface for Proximity sensors
*/
  class iProximity
  {
  public:
    virtual ~iProximity() {}

    /**
     * Get proximity value from sensor data.
     *
     * @return Proximity value as volts.
     */
    virtual float getValue() = 0;
  };
}
