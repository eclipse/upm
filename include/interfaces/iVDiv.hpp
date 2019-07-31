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
 * @brief Interface for Voltage Dividing sensors
*/
  class iVDiv
  {
  public:
    virtual ~iVDiv() {}

    /**
     * Gets the conversion value from the sensor
     *
     * @return ADC conversion value in volts
     */
    virtual unsigned int getValue() = 0;

  };
}
