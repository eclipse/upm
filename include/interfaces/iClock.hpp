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
 * @brief Interface for Real Time Clock (RTC) Modules
*/
  class iClock
  {
  public:
    virtual ~iClock() {}

    /**
     * Returns the time since epoch in seconds
     *
     * @return time since epoch in seconds
     */
    virtual unsigned long getTime() = 0;
  };
}
