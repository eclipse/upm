#include "noelstemplightreader.hpp"

#include <algorithm>

using namespace upm;

std::map<std::string, float> NoelsTempLightReader::LightForSources(std::vector<std::string> sources)
{
    std::map<std::string, float> ret;

    if (std::find(sources.begin(), sources.end(), "light0") != sources.end())
        ret["light0"] = 1.1;
    if (std::find(sources.begin(), sources.end(), "light1") != sources.end())
        ret["light1"] = 2.2;

    return ret;
}

std::map<std::string, float> NoelsTempLightReader::TemperatureForSources(std::vector<std::string> sources)
{
    std::map<std::string, float> ret;

    if (std::find(sources.begin(), sources.end(), "temperature0") != sources.end())
        ret["temperature0"] = 10.1;
    if (std::find(sources.begin(), sources.end(), "temperature1") != sources.end())
        ret["temperature1"] = 20.2;

    return ret;
}
