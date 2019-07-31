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
 * @brief Interface for Button sensors
*/
  class iButton
  {
  public:
    virtual ~iButton() {}

    /**
     * Gets the current button press state.
     *
     * @returns Button state
     */
    virtual bool isPressed() = 0;
  };
}
