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
 * @brief Interface for Electrical Conductivity (EC) Sensors
*/
  class iEC
  {
  public:
    virtual ~iEC() {}

    /**
     * Get computed EC (ms/cm) value from the sensor.
     *
     * @return EC value in ms/cm.
     */
    virtual float getECValue() = 0;

  };
}
