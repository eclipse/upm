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
    class iAcceleration
    {
        public:
            virtual ~iAcceleration() {}

            /**
             * Get acceleration values on X, Y and Z axis.
             * v[0] = X, v[1] = Y, v[2] = Z
             * 
             * @return vector of 3 floats containing acceleration on each axis in Gs
             */
            virtual std::vector<float> getAcceleration() = 0;
    };
} // upm
