/*
 * Author: Wouter van Verre <wouter.van.verre@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#define UPM_CHECK_MRAA_SUCCESS(ret, msg)                                         \
    if ((ret) != mraa::SUCCESS) {                                                 \
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + ": " + msg); \
    }

#define UPM_GOTO_ON_MRAA_FAIL(ret, target) \
    if ((ret) != mraa::SUCCESS) {           \
        goto target;                       \
    }
