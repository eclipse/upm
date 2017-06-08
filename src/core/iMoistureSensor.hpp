#pragma once

#include <map>
#include <vector>

#include "iSensorType.hpp"

namespace upm
{
    class iMoistureSensor : public virtual iSensorType
    {
        public:
            virtual std::map<std::string, float> MoistureAll();
            virtual std::map<std::string, float> MoistureForSource(std::string source);
            virtual std::map<std::string, float> MoistureForSources(std::vector<std::string> sources) = 0;

            iMoistureSensor();

            /**
             * Read and return all values for this sensor as JSON
             *
             * @return JSON string of moisture values
             */
            virtual std::string JsonMoisture() const;

        private:
            static std::string _JsonMoisture(iUpmObject * inst);
    };
}
