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

#include <stdexcept>

namespace upm
{
  enum class PressureUnit { PA, BAR, ATM, TORR, PSI };
/**
* @brief Interface for Pressure Measuring Sensors
*/
  class iPressure
  {
  public:
    virtual ~iPressure() {}

    /**
     * Measures applied pressure
     * 
     * @return gets pressure value in Pa
     */
    virtual float getPressure() = 0;

    /**
     * Convert pressure value from Pascal(default) to one
     * of the following:
     *
     * 1. Bar
     * 2. Standard atmosphere
     * 3. Torr
     * 4. Pounds per square inch
     *
     * @param paValue Pa pressure value
     * @param unit The pressure unit for the conversion.
     * @return The converted pressure value
     */
    static float convertPaTo(float paValue, PressureUnit unit)
    {
      float convertedValue = paValue;

      switch (unit)
      {
        case PressureUnit::PA:
          break;
        case PressureUnit::BAR:
          convertedValue *= 0.00001f;
          break;
        case PressureUnit::ATM:
          convertedValue *= 0.0000098692f;
          break;
        case PressureUnit::TORR:
          convertedValue *= 0.0075006f;
          break;
        case PressureUnit::PSI:
          convertedValue *= 0.0001450377f;
          break;
        default:
          throw std::invalid_argument("invalid pressure unit");
      }

      return convertedValue;
    }
  };
}
