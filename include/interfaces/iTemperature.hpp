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
  enum class TemperatureUnit { FAHRENHEIT, KELVIN, CELSIUS };

/**
* @brief Interface for Temperature Measuring Sensors
*/
  class iTemperature
  {
  public:

    virtual ~iTemperature() {}

    /**
     * Measures temperature
     * 
     * @return temperature value in degrees Celsius
     */
    virtual float getTemperature() = 0;

    /**
     * Convert temperature value from Celsius(default) to one
     * of the following:
     *
     * 1. Fahrenheit
     * 2. Kelvin
     *
     * @param celsiusValue Celsius degrees value
     * @param unit The temperature unit for the conversion.
     * @return The converted temperature value
     */
    static float convertCelsiusTo(float celsiusValue, TemperatureUnit unit)
    {
      float convertedValue = celsiusValue;

      switch (unit)
      {
        case TemperatureUnit::CELSIUS:
          break;
        case TemperatureUnit::FAHRENHEIT:
          convertedValue = celsiusValue * 1.8f + 32;
          break;
        case TemperatureUnit::KELVIN:
          convertedValue += 273.15f;
          break;
        default:
          throw std::invalid_argument("invalid temperature unit");
      }

      return convertedValue;
    }
  };
}
