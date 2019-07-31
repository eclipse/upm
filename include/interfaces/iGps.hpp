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
 * @brief Interface for GPS modules
*/
  class iGps
  {
  public:
    virtual ~iGps() {}

    /**
     * Enable or disable the device.
     *
     * @param enable true to enable the device, false otherwise.
     */
    virtual void enable(bool enable) = 0;
  };
}
