/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>

#include "ozwinterface.hpp"

namespace upm {

  /**
   * @library ozw
   * @comname Interface API for Ozwdump
   * @con uart
   *
   * @brief UPM Utility driver for OZW.
   *
   * This "driver" does not actually control any device.  It is meant
   * as an example.  It's only purpose is to support it's example
   * (ozwdump), which initializes OZW on a Z-Wave network, and then
   * prints out all Nodes (devices) and their associated valueids.
   *
   * @snippet ozw-ozwdump.cxx Interesting
   */

  class OZWDUMP : public ozwInterface {
  public:

    /**
     * ozwdump constructor
     */
    OZWDUMP();

    /**
     * OZWDUMP Destructor
     */
    ~OZWDUMP();

  protected:
  private:
  };
}
