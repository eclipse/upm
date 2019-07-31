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
 * @brief Interface for Electromagnet modules
*/
  class iElectromagnet
  {
  public:
    virtual ~iElectromagnet() {}

    /**
     * Turns the magnet on
     */
     virtual void on() = 0;
    /**
     * Turns the magnet off
     */
     virtual void off() = 0;

  };
}
