/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <unistd.h>
#include <iostream>
#include "es08a.h"
#include <signal.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
    //! [Interesting]
    upm::ES08A *servo = new upm::ES08A(5);    

    // Sets the shaft to 180, then to 90, then to 0,
    // then back to 90, and finally back to 180,
    // pausing for a second in between each angle
    servo->setAngle (180);
    std::cout << "Set angle to 180" << std::endl;
    sleep(1);
    servo->setAngle (90);
    std::cout << "Set angle to 90" << std::endl;
    sleep(1);
    servo->setAngle (0);
    std::cout << "Set angle to 0" << std::endl;
    sleep(1);
    servo->setAngle (90);
    std::cout << "Set angle to 90" << std::endl;
    sleep(1);
    servo->setAngle (180);
    std::cout << "Set angle to 180" << std::endl;
    //! [Interesting]

    std::cout << "exiting application" << std::endl;

    delete servo;

    return 0;
}
