/*
 * Author: Serban Waltter <serban.waltter@rinftech.com>
 * Copyright (c) 2018 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <vector>

namespace upm
{
    /**
     * @brief Interface for acceleration sensors
     */
    class iGas
    {
        public:
            virtual ~iGas() {}

            /**
             * Return gas concentration in PPM
             *
             * @return gas concentration in float
             */
            virtual float getConcentration() = 0;
    };
} // upm
