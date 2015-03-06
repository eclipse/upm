#pragma once

#include "mraa/common.h"

class ILightSensor
{
public:
    virtual mraa_result_t getLuxValue (uint16_t* value) = 0;
    virtual ~ILightSensor() {}
};
