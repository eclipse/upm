#pragma once
#include "mraa/common.h"

namespace upm
{

   class ITemperatureHumiditySensor
   {
   public:
       virtual mraa_result_t getTemperature (float* value) = 0;
       virtual mraa_result_t getHumidity (float* value) = 0;
       virtual bool isConfigured() = 0;
       virtual ~ITemperatureHumiditySensor() {}
   };

}

