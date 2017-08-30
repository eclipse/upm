#include <iostream>
#include <stdlib.h>
#include <string>

#include "hlg150h.hpp"
#include "iLightController.hpp"

#define HLG150H_GPIO_RELAY 21
#define HLG150H_GPIO_PWM 22

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
    upm::HLG150H lightController(HLG150H_GPIO_RELAY, HLG150H_GPIO_PWM);
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
