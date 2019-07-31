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
 * @brief Interface for PH sensors
*/
  class iPH
  {
  public:
    virtual ~iPH() {}

    /**
     * Returns the detected pH value.
     *
     * @return The pH value detected
     */
    virtual float getPH() = 0;

  };
}
