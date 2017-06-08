#include "iHumiditySensor.hpp"

using namespace upm;

std::map <std::string, float> iHumiditySensor::HumidityAll ()
{return HumidityForSources(Sources());}

float iHumiditySensor::HumidityForSource (std::string source)
{
    std::map<std::string, float> vals = HumidityForSources(std::vector<std::string>(1, source));

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
iHumiditySensor::iHumiditySensor ()
{
    AddValueSerializer(this, &_JsonHumidity);
}
std::string iHumiditySensor::JsonHumidity () const
{
    return "{" + _JsonHumidity((iHumiditySensor*)this) + "}";
}
std::string iHumiditySensor::_JsonHumidity (iUpmObject * inst)
{
    std::stringstream ss;

    /* Downcast to reference (throws if cast fails) */
    iHumiditySensor& ref = dynamic_cast<iHumiditySensor&>(*inst);

    std::map<std::string, float> data = ref.HumidityAll();

    for (std::map<std::string, float>::const_iterator it = data.begin();
            it != data.end();)
    {
        ss << "\"" << it->first << "\" : {\"value\" : " << it->second
            << ", \"units\" : \"" << ref.Unit(it->first) << "\"}";
        if (++it != data.end()) ss << "," << std::endl;
    }

    return ss.str();
}
