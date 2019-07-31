/*
 * The MIT License (MIT)
 *
 * Author: Oussema Harbi <oussema.elharbi@gmail.com>
 * Copyright (c) <2016> <Oussema Harbi>
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <signal.h>
#include <stdio.h>

#include "smartdrive.hpp"
#include "upm_utilities.h"

void
sig_handler(int signo)
{
    printf("got signal\n");
    if (signo == SIGINT) {
        printf("exiting application\n");
    }
}

int
main(int argc, char** argv)
{
    std::cout << "SmartDrive demo is starting. Please make sure drive is "
                 "connected to board"
              << std::endl;
    upm_delay(2); // Wait for 2 seconds in case you want to fix your h/w setup

    //! [Interesting]
    // Instantiate a SmartDrive connected to /dev/i2c-0 bus, using DefaultAddress
    upm::SmartDrive drive(0);

    std::cout << "Battery Voltage before motor run : " << drive.GetBattVoltage() << std::endl;
    // Set motor M1 to run for 120 seconds, with speed of 15RPM, waith for it to
    // finish and then Brake It
    drive.Run_Seconds(
    SmartDrive_Motor_ID_1, SmartDrive_Dir_Forward, 15, 120, true, SmartDrive_Action_Brake);
    std::cout << "Battery Voltage after motor run : " << drive.GetBattVoltage() << std::endl;
    // Rotate motor M2 2270 degrees, in reverse sense, with speed of 10RPM, return
    // immediately from function call
    drive.Run_Degrees(
    SmartDrive_Motor_ID_2, SmartDrive_Dir_Reverse, 10, 2270, false, SmartDrive_Action_Float);
    // While motor is running, Display its status
    drive.PrintMotorStatus(SmartDrive_Motor_ID_2);
    upm_delay(2); // Sleep for 2 seconds
    // Stop motor M2 and then finish program
    drive.StopMotor(SmartDrive_Motor_ID_2, SmartDrive_Action_BrakeHold);
    //! [Interesting]

    std::cout << "Demo complete. GoodBye" << std::endl;

    return 0;
}
