/*
 * Author: Thomas Ingleby <thomas.c.ingleby@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include "lcm1602.hpp"
#include "upm_utilities.h"

int
main(int argc, char** argv)
{
    //! [Interesting]
    upm::Lcm1602 lcd(0, 0x27);
    lcd.setCursor(0, 0);
    lcd.write("Hello World");
    //! [Interesting]
    upm_delay(3);
    lcd.setCursor(1, 2);
    lcd.write("Hello World");
    upm_delay(3);
    lcd.setCursor(2, 4);
    lcd.write("Hello World");
    upm_delay(3);
    lcd.setCursor(3, 6);
    lcd.write("Hello World");
    upm_delay(3);
    return 0;
}
