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

#include <unistd.h>
#include <iostream>
#include <stdexcept>

#include "ozwdump.hpp"

using namespace upm;
using namespace std;


OZWDUMP::OZWDUMP() :
  ozwInterface(0) // nodeID is not used
{
}

OZWDUMP::~OZWDUMP()
{
}
