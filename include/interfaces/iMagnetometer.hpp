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
    class iMagnetometer
    {
        public:
            virtual ~iMagnetometer() {}

            /**
             * Return gyroscope data in degrees per second in the form of
             * a floating point vector in micro Tesla.
             *
             * @return A floating point vector containing x, y, and z in
             * that order in micro Tesla.
             */
            virtual std::vector<float> getMagnetometer() = 0;
    };
} // upm
