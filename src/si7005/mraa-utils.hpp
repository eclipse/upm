/*
 * Author: Henry Bruce <henry.bruce@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "mraa/types.h"

class __attribute__ ((visibility("hidden"))) MraaUtils
{
public:
   static void setGpio(int pin, int level);
   static int getGpio(int pin);
};



