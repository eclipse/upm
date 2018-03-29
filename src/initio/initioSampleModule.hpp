#pragma once

#include <string>
#include <mraa/initio.hpp>

namespace upm {
    class initioSampleModule : public mraa::MraaIo {
    public:
        initioSampleModule(std::string initStr);
        ~initioSampleModule();

    private:
        //mraa_gpio_context m_gpio;
    };
}


