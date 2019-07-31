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
 * @brief Interface for Line Finder Modules
*/
  class iLineFinder
  {
  public:
    virtual ~iLineFinder() {}

    /**
     * Determines whether white has been detected
     *
     * @return True if white is detected
     */
    virtual bool whiteDetected() = 0;
    /**
     * Determines whether black has been detected
     *
     * @return True if black is detected
     */
    virtual bool blackDetected() = 0;

  };
}
