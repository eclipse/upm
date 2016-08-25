/*
 * The MIT License (MIT)
 *
 * Author: Oussema Harbi <oussema.elharbi@gmail.com>
 * Copyright (c) <2016> <Oussema Harbi>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <unistd.h>
#include <iostream>
#include "smartdrive.hpp"
#include <signal.h>

upm::SmartDrive *drive = NULL;

void
sig_handler(int signo)
{
    printf("got signal\n");
    if (signo == SIGINT) {
        printf("exiting application\n");
        if (drive != NULL)
            delete drive;
        exit (0);
    }
}

int
main(int argc, char **argv)
{
    float voltage = 0;

    std::cout << "SmartDrive demo is starting. Please make sure drive is connected to board" << std::endl;
    sleep(2); //Wait for 2 seconds in case you want to fix your h/w setup
    
    //! [Interesting]
    // Instantiate a SmartDrive connected to /dev/i2c-0 bus, using DefaultAddress
    drive = new upm::SmartDrive(0);

    std::cout << "Battery Voltage before motor run : " << drive->GetBattVoltage() << std::endl;
    //Set motor M1 to run for 120 seconds, with speed of 15RPM, waith for it to finish and then Brake It
    drive->Run_Seconds(SmartDrive_Motor_ID_1, SmartDrive_Dir_Forward, 15, 120, true, SmartDrive_Action_Brake);
    std::cout << "Battery Voltage after motor run : " << drive->GetBattVoltage() << std::endl;
    //Rotate motor M2 2270 degrees, in reverse sense, with speed of 10RPM, return immediately from function call
    drive->Run_Degrees(SmartDrive_Motor_ID_2, SmartDrive_Dir_Reverse, 10, 2270, false, SmartDrive_Action_Float);
    //While motor is running, Display its status
    drive->PrintMotorStatus(SmartDrive_Motor_ID_2);
    sleep(2); //Sleep for 2 seconds
    //Stop motor M2 and then finish program
    drive->StopMotor(SmartDrive_Motor_ID_2, SmartDrive_Action_BrakeHold);
    //! [Interesting]

    std::cout << "Demo complete. GoodBye" << std::endl;

    delete drive;
    drive = NULL;

    return 0;
}
