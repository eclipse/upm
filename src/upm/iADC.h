#pragma once
#include <stdint.h>
#include "mraa/common.h"

namespace upm
{

   class IADC
   {
   public:
       virtual mraa_result_t getValue(int input, uint16_t *value) = 0;
       virtual float convertToVolts(uint16_t value) = 0;
       virtual bool isConfigured() = 0;
       virtual ~IADC() {}
   };

}

