/*
 * Author: Gerard Vidal <gerard.vidal@ens-lyon.fr>
 * 2017 IFE ENS de Lyon
 *
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <string.h>

#include "tca9548a.hpp"

using namespace std;
using namespace upm;

/*
 * This examble is built to multiplex channel 2 and Channel 4
 *
 */

void
showAllPorts(upm::TCA9548A& multiplex)
{
    int i;
    char ports[80];
    std::string convert;
    for (i = 0; i < 8; i++) {
        if (multiplex.getPort(i))
            strcat(ports, "1.");
        else
            strcat(ports, "0.");
    }
    cout << "Ports: " << ports;
}

int
main(int argc, char** argv)
{
    // Instantiate a TCA9548A instance of i2c multiplexer
    upm::TCA9548A multiplex(0);

    // Clean
    multiplex.disableAllPorts();
    cout << "All ports disabled.";

    // Open i2c bus on multiplexer channel 4
    multiplex.setPort(4, multiplex.ENABLED, multiplex.EXCLUSIVE);
    cout << "Port 4 enabled with exclusive access.";
    showAllPorts(multiplex);

    // Close com with channel 4
    multiplex.disableAllPorts();

    // Open i2c bus on multiplexer channel 2
    multiplex.setPort(2, multiplex.ENABLED, multiplex.EXCLUSIVE);
    cout << "Port 2 enabled with exclusive access.";
    showAllPorts(multiplex);

    // close com with channel 2
    multiplex.disableAllPorts();

    // Open 2 i2c bus on multiplexer channel 4 and channel2
    multiplex.setPort(4, multiplex.ENABLED, multiplex.EXCLUSIVE);
    multiplex.setPort(2, multiplex.ENABLED, multiplex.INCLUSIVE);
    cout << "Port 4 enabled with exclusive access.";
    cout << "Port 2 enabled with exclusive access.";
    showAllPorts(multiplex);

    // close com with all channels
    multiplex.disableAllPorts();

    // Open i2c bus on all multiplexer channels
    multiplex.enableAllPorts();
    cout << "All ports enabled.";
    showAllPorts(multiplex);

    // close com with all channels
    multiplex.disableAllPorts();

    return 0;
}
