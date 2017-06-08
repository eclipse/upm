#include "iLightSensor.hpp"

using namespace upm;

std::map <std::string, float> iLightSensor::LightAll ()
{return LightForSources(Sources());}

float iLightSensor::LightForSource (std::string source)
{
    std::map<std::string, float> vals = LightForSources(std::vector<std::string>(1, source));

    if (vals.empty())
    {
        std::stringstream ss;
        ss << __FUNCTION__ << " sensor does not provide source: '"
            << source << ".  Valid sources are: {";
        std::copy(Sources().begin(), Sources().end() - 1,
                std::ostream_iterator<std::string>(ss, ", "));
        ss << Sources().back() << "}";
        throw std::invalid_argument(ss.str());
    }

    return vals[source];
}

iLightSensor::iLightSensor ()
{
    AddValueSerializer(this, &_JsonLight);
}

std::string iLightSensor::JsonLight ()
{
    return "{" + _JsonLight(dynamic_cast<iLightSensor*>(this)) + "}";
}

std::string iLightSensor::_JsonLight (iUpmObject * inst)
{
    std::stringstream ss;

    /* Downcast to reference (throws if cast fails) */
    iLightSensor& ref = dynamic_cast<iLightSensor&>(*inst);

    std::map<std::string, float> data = ref.LightAll();

    for (std::map<std::string, float>::const_iterator it = data.begin();
            it != data.end();)
    {
        ss << "\"" << it->first << "\" : {\"value\" : " << it->second
            << ", \"units\" : \"" << ref.Unit(it->first) << "\"}";
        if (++it != data.end()) ss << "," << std::endl;
    }

    return ss.str();
}
