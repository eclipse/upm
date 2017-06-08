#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>

#include "iUpmObject.hpp"
#include "iMraa.hpp"
#include "mraa/iio.hpp"
#include "mraa/uart_ow.hpp"

#define iMraaThrow(failure, protocol, fullstring) throw std::runtime_error("Failed to initialize " + \
        std::string(protocol) + " from " + std::string(fullstring) + ", cause: " + \
        std::string(failure))

using namespace upm;

template<typename Out>
void split(const std::string &s, char delim, Out result)
{
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim))
        *(result++) = item;
}

std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

uint32_t str2uint32(const std::string &number)
{
    std::string lwr = number;
    std::transform(lwr.begin(), lwr.end(), lwr.begin(), ::tolower);
    uint32_t ret = 0;
    std::stringstream ss;
    if ((lwr.compare(0, 2, "0x") == 0) &&
            (lwr.substr(2).find_first_not_of("0123456789abcdef") == std::string::npos))
    {
        ss << std::hex << lwr.substr(2);
        ss >> ret;
    }
    else if (!lwr.empty() && lwr.find_first_not_of("0123456789") == std::string::npos)
    {
        ss << lwr;
        ss >> ret;
    }
    else
        throw std::runtime_error("Cannot convert '" + number + "' to an integer");

    return ret;
}

std::map<std::string, mraa::Mode> _mraa_Mode =
{
    {"strong", mraa::MODE_STRONG},
    {"pullup", mraa::MODE_PULLUP},
    {"pulldown", mraa::MODE_PULLDOWN},
    {"hiz", mraa::MODE_HIZ},
};

std::map<std::string, mraa::Dir> _mraa_Dir =
{
    {"out", mraa::DIR_OUT},
    {"in", mraa::DIR_IN},
    {"high", mraa::DIR_OUT_HIGH},
    {"low", mraa::DIR_OUT_LOW},
};
std::map<std::string, mraa::Edge> _mraa_Edge =
{
    {"none", mraa::EDGE_NONE},
    {"both", mraa::EDGE_BOTH},
    {"rising", mraa::EDGE_RISING},
    {"falling", mraa::EDGE_FALLING},
};
std::map<std::string, mraa::InputMode> _mraa_InputMode =
{
    {"active_high", mraa::MODE_IN_ACTIVE_HIGH},
    {"avtive_low", mraa::MODE_IN_ACTIVE_LOW},
};
std::map<std::string, mraa::OutputMode> _mraa_OutputMode =
{
    {"open_drain", mraa::MODE_OUT_OPEN_DRAIN},
    {"push_pull", mraa::MODE_OUT_PUSH_PULL},
};

std::map<std::string, mraa::I2cMode> _mraa_I2cModes =
{
    {"std", mraa::I2C_STD},
    {"fast", mraa::I2C_FAST},
    {"high", mraa::I2C_HIGH},
};

iMraa::iMraa()
{
    DEBUG_MSG("XXX");

    if (mraa::init() != mraa::SUCCESS)
        throw std::runtime_error("Failed to initialize MRAA");
}

iMraa::iMraa(const std::string &init_string) : iMraa()
{
    DEBUG_MSG("XXX");

    /* Convert to lower case for processing */
    std::string lwr = init_string;
    std::transform(lwr.begin(), lwr.end(), lwr.begin(), ::tolower);

    /* Split string into protocols separated by ',' */
    std::vector<std::string> protocols = split(lwr, ',');

    for (std::vector<std::string>::const_iterator it = protocols.begin();
            it != protocols.end(); ++it)
    {
        /* Assign a reference for easier use */
        const std::string& proto_str = *it;

        /* Split proto_str into chunks separated by ':' */
        std::vector<std::string> params = split(proto_str, ':');

        /* Some basic error checking */
        if (params.size() < 2)
            throw std::runtime_error("Invalid initialization string: '" +
                    proto_str + "'.  Format is proto0:val0:val1,proto1:val0:val1");

        /* First character of first parameter defines the protocol */
        const char proto = params[0][0];

        switch (proto)
        {
            case 'a':
                {
                    /* Next chunk should be a pin # */
                    uint32_t pin = str2uint32(params[1]);
                    try {_aio.push_back(new mraa::Aio(pin));}
                    catch (std::exception &e)
                    {
                        throw std::invalid_argument("Failed to initialize Aio from string: '" +
                                proto_str + "'.  Reason: " + e.what());
                    }
                    break;
                }
            case 'g':
                {
                    /* Next chunk should be a pin # */
                    uint32_t pin = str2uint32(params[1]);
                    try { _gpio.push_back(new mraa::Gpio(pin)); }
                    catch (std::exception &e)
                    {
                        throw std::invalid_argument("Failed to initialize Gpio from string: '" +
                                proto_str + "'.  Reason: " + e.what());
                    }

                    /* Parse any additional parameters */
                    for(size_t i = 2; i < params.size(); ++i)
                    {
                        /* Gpio output modes */
                        if (_mraa_Mode.find(params[i]) != _mraa_Mode.end())
                        {
                            if (_gpio.back()->mode(_mraa_Mode[params[i]]) != mraa::SUCCESS)
                                iMraaThrow("mode(" + params[i] + ") failed.", "GPIO", proto_str);
                        }
                        /* Gpio direction options */
                        else if (_mraa_Dir.find(params[i]) != _mraa_Dir.end())
                        {
                            if (_gpio.back()->dir(_mraa_Dir[params[i]]) != mraa::SUCCESS)
                                iMraaThrow("dir(" + params[i] + ") failed.", "GPIO", proto_str);
                        }
                        /* Gpio edge types for interrupts */
                        else if (_mraa_Edge.find(params[i]) != _mraa_Edge.end())
                        {
                            if (_gpio.back()->edge(_mraa_Edge[params[i]]) != mraa::SUCCESS)
                                iMraaThrow("edge(" + params[i] + ") failed.", "GPIO", proto_str);
                        }
                        /* Gpio input modes */
                        else if (_mraa_InputMode.find(params[i]) != _mraa_InputMode.end())
                        {
                            if (_gpio.back()->inputMode(_mraa_InputMode[params[i]]) != mraa::SUCCESS)
                                iMraaThrow("inputMode(" + params[i] + ") failed.", "GPIO", proto_str);
                        }
                        /* Gpio output driver modes */
                        else if (_mraa_OutputMode.find(params[i]) != _mraa_OutputMode.end())
                        {
                            if (_gpio.back()->outputMode(_mraa_OutputMode[params[i]]) != mraa::SUCCESS)
                                iMraaThrow("outputMode(" + params[i] + ") failed.", "GPIO", proto_str);
                        }
                        /* Empty chunk? Skip it */
                        else if (params[i].empty()) {}
                        /* Unknown string?  Write to stderr and continue */
                        else std::cerr << "Unknown parameter: '" << params[i] << "' from "
                            << proto_str<< std::endl;
                    }
                    break;
                }
            case 'i':
                {
                    /* Next chunk should be a bus # */
                    uint32_t bus = str2uint32(params[1]);
                    try { _i2c.push_back(new mraa::I2c(bus)); }
                    catch (std::exception &e)
                    {
                        throw std::invalid_argument("Failed to initialize I2c from string: '" +
                                proto_str + "'.  Reason: " + e.what());
                    }
                    /* Parse any additional parameters */
                    for(size_t i = 2; i < params.size(); ++i)
                    {
                        /* I2c modes */
                        if (_mraa_I2cModes.find(params[i]) != _mraa_I2cModes.end())
                        {
                            if (_i2c.back()->frequency(_mraa_I2cModes[params[i]]) != mraa::SUCCESS)
                                iMraaThrow("frequency(" + params[i] + ") failed.", "I2C", proto_str);
                        }
                    }
                    break;
                }
            case 'o':
                break;
            case 'p': /* p:<pin>[:min_pw_us][:max_pw_us] */
                {
                    /* Next chunk should be a pin # */
                    uint32_t pin = str2uint32(params[1]);
                    try { _pwm.push_back(new mraa::Pwm(pin)); }
                    catch (std::exception &e)
                    {
                        throw std::invalid_argument("Failed to initialize Pwm from string: '" +
                                proto_str + "'.  Reason: " + e.what());
                    }
                    ///* Parse any additional parameters */
                    ///* Min pulse width in us */
                    //if (params.size() > 2)
                    //{
                    //    if (_pwm.back()->min(_mraa_I2cModes[params[i]]) != mraa::SUCCESS)
                    //            iMraaThrow("frequency(" + params[i] + ") failed.", "I2C", proto_str);
                    //}
                    break;
                }
            case 's':
                break;
            case 'u':
                break;
            case 'w':
                break;
            default:
                break;
        }
    }
    return;

}

iMraa::~iMraa()
{
    for (std::vector<mraa::Aio*>::iterator it = _aio.begin();
            it != _aio.end(); ++it) delete *it;
    for (std::vector<mraa::Gpio*>::iterator it = _gpio.begin();
            it != _gpio.end(); ++it) delete *it;
    for (std::vector<mraa::I2c*>::iterator it = _i2c.begin();
            it != _i2c.end(); ++it) delete *it;
    for (std::vector<mraa::Pwm*>::iterator it = _pwm.begin();
            it != _pwm.end(); ++it) delete *it;
}

std::string iMraa::Connections()
{
    std::stringstream ss;
    ss << "MRAA connections ("
        << _aio.size() + _gpio.size() + _i2c.size() + _iio.size() +
        _pwm.size() + _spi.size() + _uart.size() + _uartow.size()
        << "):" << std::endl;
    if (!_aio.empty()) ss << "\tAIO: " << _aio.size() << std::endl;
    if (!_gpio.empty()) ss << "\tGPIO: " << _gpio.size() << std::endl;
    if (!_i2c.empty()) ss << "\tI2C: " << _i2c.size() << std::endl;
    if (!_iio.empty()) ss << "\tIIO: " << _iio.size() << std::endl;
    if (!_pwm.empty()) ss << "\tPWM: " << _pwm.size() << std::endl;
    if (!_spi.empty()) ss << "\tSPI: " << _spi.size() << std::endl;
    if (!_uart.empty()) ss << "\tUART: " << _uart.size() << std::endl;
    if (!_uartow.empty()) ss << "\tOneWire: " << _uartow.size() << std::endl;
    return ss.str();
}
