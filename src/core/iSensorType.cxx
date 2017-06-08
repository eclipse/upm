#include "iSensorType.hpp"

#include "external/json/json.hpp"

using namespace upm;

namespace upm {
template <typename T>
void to_json(nlohmann::json& j, const SensorSource<T> &p) {
    j = nlohmann::json{{"unit", p.unit}, {"min", p.min}, {"max", p.max}, {"accuracy", p.accuracy}};
}

template <typename T>
void from_json(const nlohmann::json& j, SensorSource<T>& p) {
    p.unit = j.at("unit").get<std::string>();
    p.min = j.at("min").get<T>();
    p.max = j.at("max").get<T>();
    p.accuracy = j.at("accuracy").get<T>();
}
}

iSensorType::iSensorType()
{
    DEBUG_MSG("XXX");
}

iSensorType::~iSensorType()
{
    DEBUG_MSG("XXX");
}

std::string iSensorType::LibraryJsonSources()
{
    std::stringstream ss;
    ss << this->LibraryJson()["Sensor Class"][Name()]["Sources"] << std::endl;
    return ss.str();
}

void iSensorType::AddSource (std::string source, std::string unit)
{
    /* Add the source:unit to the map */
    _sources_2_units[source] = unit;

    /* Each call regens the sources and units vectors */
    _sources.clear();
    _units.clear();

    for(std::map<std::string, std::string>::const_iterator it = SourceMap().begin();
            it != SourceMap().end(); ++it)
    {
        _sources.push_back(it->first);
        _units.push_back(it->second);
    }

    /* Uniquify the sources vector */
    std::sort(_sources.begin(), _sources.end());
    _sources.erase(std::unique(_sources.begin(), _sources.end()), _sources.end());

    /* Uniquify the units vector */
    std::sort(_units.begin(), _units.end());
    _units.erase(std::unique(_units.begin(), _units.end()), _units.end());
}

std::map <std::string, std::string> const & iSensorType::SourceMap () const
{ return _sources_2_units;}

std::vector <std::string> const & iSensorType::Sources ()
{
    /* If there are zero sources, look to the library json descriptor file */
    if (_sources.empty())
    {
        /* Copy all commands out of the JSON */
        nlohmann::json j_sources = nlohmann::json::parse(LibraryJsonSources());
        for (nlohmann::json::iterator it = j_sources.begin(); it != j_sources.end(); ++it)
            _sources.push_back(it.key());
    }

    return _sources; }

std::vector <std::string> const & iSensorType::Units () const
{ return _units; }

std::string const iSensorType::Unit (std::string source) const
{
    std::map<std::string, std::string>::const_iterator it =
        SourceMap().find(source);
    if (it != SourceMap().end())
        return it->second;
    else
        return "";
}

bool iSensorType::HasSource (std::string source)
{
    return std::find(Sources().begin(), Sources().end(), source) != Sources().end();
}

bool iSensorType::HasUnit (std::string unit) const
{
    return std::find(Units().begin(), Units().end(), unit) != Units().end();
}

std::string iSensorType::JsonDefinition ()
{
    std::stringstream ss;

    ss << "{" << std::endl
        << "  \"name\" : \"" << Name() << "\"," << std::endl
        << "  \"description\" : \"" << Description() << "\"";

    if (!SourceMap().empty())
    {
        ss << "," << std::endl << "  \"sources\" :" << std::endl << "  {" << std::endl;
        for(std::map<std::string, std::string>::const_iterator it = SourceMap().begin();
                it != SourceMap().end();)
        {
            ss << "    \"" << it->first << "\" : {\"units\" : \"" << it->second << "\"}";
            if (++it != SourceMap().end())
                ss << "," << std::endl;
        }
        ss << std::endl << "  }";
    }
    ss << std::endl << "}";
    return ss.str();
}

std::string iSensorType::JsonValues ()
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
    return ss.str();
}

/* Since nlohmann::json has only been fwd declared (not included in a header),
 *  json is a partial type.  Because of this explicitly instantiate basic
 *  types.
*/
template void upm::to_json(nlohmann::json& j, const SensorSource<int>& p);
template void upm::from_json(const nlohmann::json& j, SensorSource<int>& p);
template void upm::to_json(nlohmann::json& j, const SensorSource<float>& p);
template void upm::from_json(const nlohmann::json& j, SensorSource<float>& p);
template void upm::to_json(nlohmann::json& j, const SensorSource<double>& p);
template void upm::from_json(const nlohmann::json& j, SensorSource<double>& p);
