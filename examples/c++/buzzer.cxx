/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>

#include "buzzer.hpp"
#include "buzzer_tones.h"
#include "upm_utilities.h"

int
main(int argc, char** argv)
{
    //! [Interesting]
    int chord[] = { BUZZER_DO, BUZZER_RE, BUZZER_MI, BUZZER_FA, BUZZER_SOL, BUZZER_LA, BUZZER_SI };

    // create Buzzer instance
    upm::Buzzer sound(32);
    // print sensor name
    std::cout << sound.name() << std::endl;

    // play each sound (DO, RE, MI, etc...) for .5 seconds, pausing
    // for 0.1 seconds between notes
    for (int chord_ind = 0; chord_ind < 7; chord_ind++) {
        std::cout << sound.playSound(chord[chord_ind], 500000) << std::endl;
        upm_delay_ms(100);
    }
    //! [Interesting]

    std::cout << "exiting application" << std::endl;

    return 0;
}
