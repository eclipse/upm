#pragma once
#include <stdint.h>
#include "mraa/common.h"

namespace upm
{

   class IPressureSensor
   {
   public:
       virtual mraa_result_t getPressure (int32_t *value) = 0;
       virtual float getTemperature () = 0;
       virtual bool isConfigured() = 0;
       virtual bool isAvailable() = 0;
       virtual ~IPressureSensor() {}
   };

}

