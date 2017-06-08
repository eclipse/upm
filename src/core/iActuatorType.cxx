#include "iActuatorType.hpp"
#include "external/json/json.hpp"

using namespace upm;

namespace upm {

template <typename T>
void to_json(nlohmann::json& j, const ActuatorSink<T> &p) {
    j = nlohmann::json{{"unit", p.unit}, {"min", p.min}, {"max", p.max}, {"accuracy", p.accuracy}};
}

template <typename T>
void from_json(const nlohmann::json& j, ActuatorSink<T>& p) {
    p.unit = j.at("unit").get<std::string>();
    p.min = j.at("min").get<T>();
    p.max = j.at("max").get<T>();
    p.accuracy = j.at("accuracy").get<T>();
}

template <typename T>
void to_json(nlohmann::json& j, const ActuatorCommand<T> &p) {
    j = nlohmann::json{{"command", p.command}, {"unit", p.unit}, {"value", p.value}};
}

template <typename T>
void from_json(const nlohmann::json& j, ActuatorCommand<T>& p) {
    p.command= j.at("command").get<std::string>();
    p.unit = j.at("unit").get<std::string>();
    p.value = j.at("value").get<T>();
}

template <class T>
std::ostream &operator<<(std::ostream& os, const ActuatorCommand<T>& ac)
{
    nlohmann::json jcmd = ac;
    return os << jcmd;
}

}

iActuatorType::iActuatorType()
{
    DEBUG_MSG("XXX");
}

iActuatorType::~iActuatorType()
{
    DEBUG_MSG("XXX");
}

std::string iActuatorType::LibraryJsonCommands()
{
    std::stringstream ss;
    ss << this->LibraryJson()["Sensor Class"][Name()]["Commands"] << std::endl;
    return ss.str();
}

void iActuatorType::AddCommand (std::string source, std::string unit)
{
    /* Add the source:unit to the map */
    _sinks_2_units[source] = unit;

    /* Each call regens the sinks and units vectors */
    _sinks.clear();
    _units.clear();

    for(std::map<std::string, std::string>::const_iterator it = CommandMap().begin();
            it != CommandMap().end(); ++it)
    {
        _sinks.push_back(it->first);
        _units.push_back(it->second);
    }

    /* Uniquify the sinks vector */
    std::sort(_sinks.begin(), _sinks.end());
    _sinks.erase(std::unique(_sinks.begin(), _sinks.end()), _sinks.end());

    /* Uniquify the units vector */
    std::sort(_units.begin(), _units.end());
    _units.erase(std::unique(_units.begin(), _units.end()), _units.end());
}

std::map <std::string, std::string> const & iActuatorType::CommandMap () const
{ return _sinks_2_units;}

std::vector <std::string> const & iActuatorType::Commands ()
{
    /* If there are zero sinks, look to the library json descriptor file */
    if (_sinks.empty())
    {
        /* Copy all commands out of the JSON */
        nlohmann::json j_sinks = nlohmann::json::parse(LibraryJsonCommands());
        for (nlohmann::json::iterator it = j_sinks.begin(); it != j_sinks.end(); ++it)
            _sinks.push_back(it.key());
    }

    return _sinks;
}

std::vector <std::string> const & iActuatorType::Units () const
{ return _units; }

std::string const iActuatorType::Unit (std::string source) const
{
    std::map<std::string, std::string>::const_iterator it =
        CommandMap().find(source);
    if (it != CommandMap().end())
        return it->second;
    else
        return "";
}

bool iActuatorType::HasCommand (std::string source)
{
    return std::find(Commands().begin(), Commands().end(), source) != Commands().end();
}

bool iActuatorType::HasUnit (std::string unit) const
{
    return std::find(Units().begin(), Units().end(), unit) != Units().end();
}

std::string iActuatorType::JsonDefinition ()
{
    std::stringstream ss;

    ss << "{" << std::endl
        << "  \"name\" : \"" << Name() << "\"," << std::endl
        << "  \"description\" : \"" << Description() << "\"";

    if (!CommandMap().empty())
    {
        ss << "," << std::endl << "  \"sinks\" :" << std::endl << "  {" << std::endl;
        for(std::map<std::string, std::string>::const_iterator it = CommandMap().begin();
                it != CommandMap().end();)
        {
            ss << "    \"" << it->first << "\" : {\"units\" : \"" << it->second << "\"}";
            if (++it != CommandMap().end())
                ss << "," << std::endl;
        }
        ss << std::endl << "  }";
    }
    ss << std::endl << "}";
    return ss.str();
}

void iActuatorType::JsonCommands(std::string commands)
{
    std::stringstream ss;
    ss << "{" << std::endl;
    if (!_child_value_serializers.empty())
    {
        /* Iterate over each serializer method */
        for (std::map<t_getJson, iUpmObject*>::const_iterator it = _child_value_serializers.begin();
                it != _child_value_serializers.end();)
        {
            // Call static method to serialize data from the derived classes
            ss << it->first(it->second);
            if (++it != _child_value_serializers.end()) ss << ",";
            ss << std::endl;
        }
    }
    ss << "}";

    /* TODO Change this method to a command method */
}

/* Since nlohmann::json has only been fwd declared (not included in a header),
 *  json is a partial type.  Because of this explicitly instantiate basic
 *  types.
*/
template void upm::to_json(nlohmann::json& j, const ActuatorSink<int>& p);
template void upm::from_json(const nlohmann::json& j, ActuatorSink<int>& p);
template void upm::to_json(nlohmann::json& j, const ActuatorSink<float>& p);
template void upm::from_json(const nlohmann::json& j, ActuatorSink<float>& p);
template void upm::to_json(nlohmann::json& j, const ActuatorSink<double>& p);
template void upm::from_json(const nlohmann::json& j, ActuatorSink<double>& p);

template void upm::to_json(nlohmann::json& j, const ActuatorCommand<int>& p);
template void upm::from_json(const nlohmann::json& j, ActuatorCommand<int>& p);
template void upm::to_json(nlohmann::json& j, const ActuatorCommand<float>& p);
template void upm::from_json(const nlohmann::json& j, ActuatorCommand<float>& p);
template void upm::to_json(nlohmann::json& j, const ActuatorCommand<double>& p);
template void upm::from_json(const nlohmann::json& j, ActuatorCommand<double>& p);

template std::ostream &upm::operator<<(std::ostream& os, const ActuatorCommand<float>& ac);
