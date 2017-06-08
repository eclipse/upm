#include "iCO2Sensor.hpp"

using namespace upm;

std::map <std::string, float> iCO2Sensor::CO2All ()
{return CO2ForSources(Sources());}

float iCO2Sensor::CO2ForSource(std::string source)
{
    std::map<std::string, float> vals = CO2ForSources(std::vector<std::string>(1, source));

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

iCO2Sensor::iCO2Sensor ()
{
    AddValueSerializer(this, &_JsonCO2);
}

std::string iCO2Sensor::JsonCO2 () const
{
    return "{" + _JsonCO2((iCO2Sensor*)this) + "}";
}

std::string iCO2Sensor::_JsonCO2 (iUpmObject * inst)
{
    std::stringstream ss;

    /* Downcast to reference (throws if cast fails) */
    iCO2Sensor& ref = dynamic_cast<iCO2Sensor&>(*inst);

    std::map<std::string, float> data = ref.CO2All();

    for (std::map<std::string, float>::const_iterator it = data.begin();
            it != data.end();)
    {
        ss << "\"" << it->first << "\" : {\"value\" : " << it->second
            << ", \"units\" : \"" << ref.Unit(it->first) << "\"}";
        if (++it != data.end()) ss << "," << std::endl;
    }

    return ss.str();
}
