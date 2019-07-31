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
* @brief Interface for Motion Sensors
*/
  class iMotion
  {
  public:
    virtual ~iMotion() {}

    /**
     * Detects motion
     * 
     * @return true if motion detected
     */
    virtual bool motionDetected() = 0;
  };
}
