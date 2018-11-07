#include <iostream>
#include <stdlib.h>
#include <string>

#include "ds1808lc.hpp"

#define EDISON_I2C_BUS 1   // Edison I2C-1
#define DS1808_GPIO_PWR 15 // Edison GP165

void
printState(upm::ILightController& lightController)
{
    if (lightController.isPowered()) {
        std::cout << "Light is powered, brightness = " << lightController.getBrightness()
                  << std::endl;
    } else {
        std::cout << "Light is not powered." << std::endl;
    }
}

int
main(int argc, char** argv)
{
    //! [Interesting]
    upm::DS1808LC lightController(DS1808_GPIO_PWR, EDISON_I2C_BUS);
    std::cout << "Existing state: ";
    printState(lightController);
    if (argc == 2) {
        std::string arg = argv[1];
        int brightness = ::atoi(argv[1]);
        if (brightness > 0) {
            lightController.setPowerOn();
            lightController.setBrightness(brightness);
        } else
            lightController.setPowerOff();
    }
    std::cout << "Now: ";
    printState(lightController);
    //! [Interesting]
    return 0;
}
