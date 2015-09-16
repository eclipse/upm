#pragma once
#include "mraa/common.h"
#include "iModuleStatus.h"

namespace upm
{
/**
 * @brief Interface for Temperature/Humidity Sensors
 */

   class ITemperatureHumiditySensor : public IModuleStatus
   {
   public:
       virtual mraa_result_t getTemperature (float* value) = 0;
       virtual mraa_result_t getHumidity (float* value) = 0;
       virtual ~ITemperatureHumiditySensor() {}
   };

}

