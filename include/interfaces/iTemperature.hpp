/*
 * Author: Mihai Stefanescu <mihai.stefanescu@rinftech.com>
 * Copyright (c) 2018 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
