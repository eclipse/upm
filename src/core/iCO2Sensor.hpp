#pragma once

#include <map>
#include <vector>

#include "iSensorType.hpp"

namespace upm
{
    /**
     * iCO2Sensor abstract class.
     *
     * Provides a common interface for all sensors which detect co2.
     */
    class iCO2Sensor : public virtual upm::iSensorType
    {
        public:
            /**
             * Read and return all values for this sensor as a map of sources
             * to values.
             *
             * @return Map of sources to values.
             */
            virtual std::map<std::string, float> CO2All();

            /**
             * Read and return a single value from the source provided
             *
             * @param source Target source to read
             *
             * @throws std::invalid_argument If source is NOT valid for this sensor
             *
             * @return Map of sources to values.
             */
            virtual float CO2ForSource(std::string source);

            /**
             * Read and return all values for this sensor for the provided
             * vector of sources.
             *
             * @param sources Vector of sources to read
             *
             * @return Map of sources to values.
             */
            virtual std::map<std::string, float> CO2ForSources(std::vector<std::string> sources) = 0;

            /**
             * Add a pointer to this type and a proxy function pointer for
             * serializing all values from this sensor type.
             */
            iCO2Sensor();

            /**
             * Read and return all values for this sensor as JSON
             *
             * @return JSON string of co2 values
             */
            virtual std::string JsonCO2() const;

        private:
            /**
             * Provide a means to read and serialize values from this sensor
             * as a static method.  This method, along with a pointer to the
             * class can be called from a base class
             *
             * @param inst Instance of iCO2Sensor to call _JsonCO2 on
             *
             * @return JSON string of co2 values (minus wrapping '{' and '}'
             */
            static std::string _JsonCO2(iUpmObject * inst);
    };
}
