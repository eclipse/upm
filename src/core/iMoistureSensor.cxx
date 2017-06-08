#include "iMoistureSensor.hpp"

using namespace upm;

std::map <std::string, float> iMoistureSensor::MoistureAll ()
{return MoistureForSources(Sources());}

std::map <std::string, float> iMoistureSensor::MoistureForSource (std::string source)
{ return MoistureForSources(std::vector<std::string>(1, source)); }

iMoistureSensor::iMoistureSensor ()
{
    AddValueSerializer(this, &_JsonMoisture);
}

std::string iMoistureSensor::JsonMoisture () const
{
    return "{" + _JsonMoisture((iMoistureSensor*)this) + "}";
}

std::string iMoistureSensor::_JsonMoisture (iUpmObject * inst)
{
    std::stringstream ss;

    /* Downcast to reference (throws if cast fails) */
    iMoistureSensor& ref = dynamic_cast<iMoistureSensor&>(*inst);

    std::map<std::string, float> data = ref.MoistureAll();

    for (std::map<std::string, float>::const_iterator it = data.begin();
            it != data.end();)
    {
        ss << "\"" << it->first << "\" : {\"value\" : " << it->second
            << ", \"units\" : \"" << ref.Unit(it->first) << "\"}";
        if (++it != data.end()) ss << "," << std::endl;
    }

    return ss.str();
}
