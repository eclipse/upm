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
 * @brief Interface for Oxidation/Reduction/Potential (ORP) Sensors
*/
  class iOrp
  {
  public:
    virtual ~iOrp() {}

    /**
     * Get computed ORP (in millivolts) value from the
     * sensor.
     *
     * @return ORP value in millivolts
     */
     virtual float getORPValue() = 0;
  };
}
