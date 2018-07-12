#include <iostream>
#include <vector>

#include "abp.hpp"
#include "lm35.hpp"

int
main()
{
    std::vector<upm::iTemperature*> tempSensors{ new upm::LM35(0),
                                                 new upm::ABP(0, ABP_DEFAULT_ADDRESS) };

    for (auto& sensor : tempSensors) {
        float celsiusTemp = sensor->getTemperature();
        std::cout << "Temperature in Celsius degrees: " << celsiusTemp << std::endl;
        std::cout << "Temperature in Kelvin: "
                  << upm::iTemperature::convertCelsiusTo(celsiusTemp, upm::TemperatureUnit::KELVIN)
                  << std::endl;
    }

    for (auto& sensor : tempSensors) {
        delete sensor;
    }

    return 0;
}
