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
  enum class DistanceUnit { CM, INCH };
/**
* @brief Interface for Distance Measuring Sensors
*/
  class iDistance
  {
  public:
    virtual ~iDistance() {}

    /**
     * Get distance from sensor
     * 
     * @return Returns the distance in cm
     */
    virtual float getDistance() = 0;
    /**
      * Convert distance value from Cm(default) to one
      * of the following:
      *
      * 1. Inch
      *
      * @param cmValue Cm distance value
      * @param unit The distance unit for the conversion.
      * @return The converted distance value
      */
    static float convertCmTo(float cmValue, DistanceUnit unit)
    {
      float convertedValue = cmValue;

      switch (unit)
      {
        case DistanceUnit::CM:
          break;
        case DistanceUnit::INCH:
          convertedValue *= 0.3937f;
          break;
        default:
          throw std::invalid_argument("invalid distance unit");
      }

      return convertedValue;
    }
  };
}
