#pragma once
#include <stdint.h>
#include "mraa/common.h"

namespace upm
{

   class ILightSensor
   {
   public:
       virtual mraa_result_t getValue (uint16_t* value) = 0;
       virtual float convertToLux (uint16_t value) = 0;
       virtual mraa_result_t reset() = 0;
       virtual bool isConfigured() = 0;
       virtual ~ILightSensor() {}
   };

}

