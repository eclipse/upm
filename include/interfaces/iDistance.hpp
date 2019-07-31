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
