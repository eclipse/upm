#include <iostream>
#include <string>
#include <stdexcept>

#include "initioSampleModule.hpp"

using namespace upm;
using namespace std;

initioSampleModule::initioSampleModule(std::string initStr)  : mraa::MraaIo(initStr)
{
    // Do some processing here
    //std::cout << "111 " << leftoverStr << std::endl;
}

initioSampleModule::~initioSampleModule()
{
}
