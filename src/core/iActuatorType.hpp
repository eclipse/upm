#pragma once

#include "iUpmObject.hpp"

/* I/O fwd declaration header */
#include <iosfwd>

namespace upm
{
    template <typename T>
    struct ActuatorSink
    {
        std::string unit;
        T min;
        T max;
        T accuracy;
    };

    template <class T>
    void to_json(nlohmann::json& j, const ActuatorSink<T>& p);

    template <class T>
    void from_json(const nlohmann::json& j, ActuatorSink<T>& p);

    template <typename T>
    struct ActuatorCommand
    {
        std::string command;
        std::string unit;
        T value;
    };

    template <class T>
    void to_json(nlohmann::json& j, const ActuatorCommand<T>& p);

    template <class T>
    void from_json(const nlohmann::json& j, ActuatorCommand<T>& p);

    template <class T>
    std::ostream &operator<<(std::ostream&, const ActuatorCommand<T>&);

    /**
     * Forward declaration of iActuatorType class for ostream usage
     */
    class iActuatorType;

    /**
     * iActuatorType abstract class.
     *
     * Provides a common interface for actuator classes.  This interface is
     * meant to be used by derived actuator categories.
     *
     * For example, iTemperatureSensor or iLightSensor
     *
     */
    class iActuatorType : public virtual iUpmObject
    {
        public:
            iActuatorType();

            /**
             * Allow derived classes to provide their own destructor
             */
            virtual ~iActuatorType();

            std::string LibraryJsonCommands();

            /**
             * Return a map reference of sinks to units.  This map can
             * be used to get the units for a actuator value given a source.
             *
             * @return Map of sinks to units.
             */
            const std::map<std::string, std::string>& CommandMap() const;

            /**
             * Return a vector of all known sinks for this actuator.
             *
             * @return Vector of sinks
             */
            const std::vector<std::string>& Commands();

            /**
             * Return a vector of all known units for this actuator.
             *
             * @return Vector of units
             */
            const std::vector<std::string>& Units() const;

            /**
             * Return a unit string given a single source.
             *
             * @return Corresponding unit for source or empty string if not found.
             */
            virtual const std::string Unit(std::string source) const;

            /**
             * Determine if this actuator has a given source.
             *
             * @return True if source string is available, fails otherwise.
             */
            virtual bool HasCommand(std::string source);

            /**
             * Determine if this actuator has a given unit.
             *
             * @return True if unit string is available, fails otherwise.
             */
            virtual bool HasUnit(std::string unit) const;

            /**
             * Return a JsonDefinition string which defines this actuator
             *
             * @return JsonDefinition-encoded string.
             */
            virtual std::string JsonDefinition();

            /**
             * @brief Json string of actuator data.
             *
             * The JsonValues call serializes ALL values returned by this actuator
             * as a Json string.
             *
             * Example:
             *
             *      "Temperature" : {"value" : 25.0, "units" : "C"}
             *      "Acceleration" : {"value" : 1.1, "units" : "m/s^2"}
             *
             *  @return Json-encoded string.
             */
            virtual void JsonCommands(std::string commands);

        private:
            /**
             * Sensor source vector
             */
            std::vector<std::string> _sinks;

            /**
             * Sensor unit vector
             */
            std::vector<std::string> _units;

            /**
             * Mapping of sinks to units
             */
            std::map<std::string, std::string> _sinks_2_units;

        protected:
            /**
             * Mapping of sinks to units
             */
            void AddCommand(std::string source, std::string unit);

    };
}
