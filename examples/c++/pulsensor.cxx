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

#include <signal.h>
#include <stdio.h>

#include "pulsensor.hpp"
#include "upm_utilities.h"

using namespace upm;

int doWork = 0;

void
sig_handler(int signo)
{
    printf("got signal\n");
    if (signo == SIGINT) {
        printf("exiting application\n");
        doWork = 1;
    }
}


//! [Interesting]
class mycb : public virtual Callback
{
    public:
        virtual void run(clbk_data arg)
        {
            printf("callback data (%d)\n", arg.is_heart_beat);
        }
};


int
main(int argc, char** argv)
{
    mycb cb;
    //! [Interesting]
    Pulsensor sensor(&cb);

    sensor.start_sampler();
    while (!doWork) {
        upm_delay_us(5);
    }
    sensor.stop_sampler();
    //! [Interesting]
    return 0;
}
