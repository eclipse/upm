/*
 * Author: Nandkishor Sonar <Nandkishor.Sonar@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * LIGHT-TO-DIGITAL CONVERTER [TAOS-TSL2561]
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>
#include <stdlib.h>

#include "tsl2561.hpp"

int
main(int argc, char** argv)
{
    int loopCount = 100;

    //! [Interesting]
    if (argc < 2) {
        printf("Provide loop count \n");
    } else {
        loopCount = atoi(argv[1]);
    }
    upm::TSL2561 sensor;

    for (int i = 0; i < loopCount; i++) {
        fprintf(stdout, "Lux = %d\n", sensor.getLux());
    }

    //! [Interesting]

    return (0);
}
