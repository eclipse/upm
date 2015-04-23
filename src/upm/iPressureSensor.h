#pragma once
#include <stdint.h>
#include "mraa/common.h"

namespace upm
{

   class IPressureSensor
   {
   public:
       virtual mraa_result_t getPressure (int32_t *value) = 0;
       virtual mraa_result_t getTemperature (int16_t *value) = 0;
       virtual bool isConfigured() = 0;
       virtual ~IPressureSensor() {}
   };

}

