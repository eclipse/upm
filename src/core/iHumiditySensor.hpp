#pragma once

#include <map>
#include <vector>

#include "iSensorType.hpp"

namespace upm
{
    /**
     * iHumiditySensor abstract class.
     *
     * Provides a common interface for all sensors which detect humidity.
     */
    class iHumiditySensor : public virtual iSensorType
    {
        public:
            /**
             * Read and return all values for this sensor as a map of sources
             * to values.
             *
             * @return Map of sources to values.
             */
            virtual std::map<std::string, float> HumidityAll();

            /**
             * Read and return a single value from the source provided
             *
             * @param source Target source to read
             *
             * @throws std::invalid_argument If source is NOT valid for this sensor
             *
             * @return Map of sources to values.
             */
            virtual float HumidityForSource(std::string source);

            /**
             * Read and return all values for this sensor for the provided
             * vector of sources.
             *
             * @param sources Vector of sources to read
             *
             * @return Map of sources to values.
             */
            virtual std::map<std::string, float> HumidityForSources(std::vector<std::string> sources) = 0;

            /**
             * Add a pointer to this type and a proxy function pointer for
             * serializing all values from this sensor type.
             */
            iHumiditySensor();

            /**
             * Read and return all values for this sensor as JSON
             *
             * @return JSON string of humidity values
             */
            virtual std::string JsonHumidity() const;

        private:
            /**
             * Provide a means to read and serialize values from this sensor
             * as a static method.  This method, along with a pointer to the
             * class can be called from a base class
             *
             * @param inst Instance of iHumiditySensor to call _JsonHumidity on
             *
             * @return JSON string of humidity values (minus wrapping '{' and '}'
             */
            static std::string _JsonHumidity(iUpmObject * inst);
    };
}
