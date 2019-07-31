/*
 * Author: Sarah Knepper <sarah.knepper@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>

#include "relay.hpp"
#include "upm_utilities.h"

int
main(int argc, char** argv)
{
    // This example uses GPIO 0
    //! [Interesting]

    // Create the relay switch object using GPIO pin 0
    upm::Relay relay(0);

    // Close and then open the relay switch 3 times,
    // waiting one second each time.  The LED on the relay switch
    // will light up when the switch is on (closed).
    // The switch will also make a noise between transitions.
    for (int i = 0; i < 3; i++) {
        relay.on();
        if (relay.isOn())
            std::cout << relay.name() << " is on" << std::endl;
        upm_delay(1);
        relay.off();
        if (relay.isOff())
            std::cout << relay.name() << " is off" << std::endl;
        upm_delay(1);
    }

    // Delete the relay switch object
    //! [Interesting]

    return 0;
}
