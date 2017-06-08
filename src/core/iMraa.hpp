#pragma once

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>

#include "iUpmObject.hpp"
#include "mraa.hpp"
#include "mraa/iio.hpp"
#include "mraa/uart_ow.hpp"

namespace upm
{
    class iMraa : public virtual iUpmObject
    {
        public:
            iMraa();

            iMraa(const std::string &init_string);

            virtual ~iMraa();

            std::string Connections();

            //
            //            template <typename T>
            //            T& Connection(std::string label);
            //
            //            template <typename T>
            //            bool DefineConnections(std::vector<std::string> labels)
            //            {
            //                if (std::is_same<T, mraa::Aio>::value)
            //                {
            //                }
            //                else if (std::is_same<T, mraa::Gpio>::value) {}
            //                else if (std::is_same<T, mraa::I2c>::value)
            //                {
            //                }
            //
            //                return false;
            //            }
            //private:
            std::vector<mraa::Aio*> _aio;
            std::vector<mraa::Gpio*> _gpio;
            std::vector<mraa::I2c*> _i2c;
            std::vector<mraa::Iio*> _iio;
            std::vector<mraa::Pwm*> _pwm;
            std::vector<mraa::Spi*> _spi;
            std::vector<mraa::Uart*> _uart;
            std::vector<mraa::UartOW*> _uartow;
    };
}
