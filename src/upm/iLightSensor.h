#pragma once
#include <stdint.h>
#include "mraa/common.h"

namespace upm
{

   class ILightSensor
   {
   public:
       virtual mraa_result_t getLuxValue (uint16_t* value) = 0;
       virtual bool isConfigured() = 0;
       virtual ~ILightSensor() {}
   };

}

