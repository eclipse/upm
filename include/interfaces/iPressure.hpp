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
  enum class PressureUnit { PA, BAR, ATM, TORR, PSI };
/**
* @brief Interface for Pressure Measuring Sensors
*/
  class iPressure
  {
  public:
    virtual ~iPressure() {}
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
    static float convertPaTo(float paValue, PressureUnit unit);
  };

  float iPressure::convertPaTo(float paValue, PressureUnit unit)
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
}
