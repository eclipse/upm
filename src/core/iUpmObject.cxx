#include <dlfcn.h>
#include <typeinfo>
#include <cxxabi.h>
#include <string.h>
#include <iostream>
#include <regex>
#include <fstream>


#include "external/json/json.hpp"
#include "iUpmObject.hpp"
#include "upm_library_globals.hpp"

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define THROW_RUNTIME(msg) throw std::runtime_error(std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + " [" + std::string(__FUNCTION__) + "] " + msg)

extern const char* DATA_DIRECTORY;
static std::string DATA_DIRECTORY_STR = DATA_DIRECTORY;

using namespace upm;
using json = nlohmann::json;

/* Locate method to test if a file exists */
static bool exists(const std::string& filename, bool throw_exception = false)
{
    std::ifstream f;

    if (throw_exception)
        f.exceptions(std::ifstream::failbit);

    try {
        f.open(filename.c_str());
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error("'" + filename + "' does not exist or is not accessible.");
    }

    return f.good();
}

/* Always create an instance of the _json pointer (empty) */
iUpmObject::iUpmObject()
{
    DEBUG_MSG("XXX");
}

iUpmObject::~iUpmObject()
{
    DEBUG_MSG("XXX");

    if (_json)
    {
        delete _json;
        _json = NULL;
    }
}

void iUpmObject::initFromJsonLibDesc(std::string path_to_json_file)
{
    /* Throw if the file exists (as is, no realpath) */
    exists(path_to_json_file, true);

    /* Next, convert to a realpath */
    const char * rp = realpath(path_to_json_file.c_str(), NULL);
    if (!rp)
        throw std::runtime_error("'" + path_to_json_file + "' does not exist or is not accessible.");

    std::string realpath_json_file(rp);

    /* If this method is getting called ALWAYS delete and re-init */
    if (_json)
    {
        delete _json;
        _json = NULL;
    }

    _json = new json;

    /* Does the provided file exist?  Is it readable? */
    std::string json_str;
    std::ifstream t(realpath_json_file);
    json_str.assign((std::istreambuf_iterator<char>(t)),
            std::istreambuf_iterator<char>());

    /* Parse the file as JSON */
    *_json = _json->parse(json_str.c_str(), nullptr, false);

    /* Set the library JSON filename */
    _LibraryJsonFilename = realpath_json_file;

    /* Run any additional deserializers available on the JSON */
    if (!_child_json_deserializers.empty())
    {
        std::stringstream ss;
        /* Iterate over each serializer method */
        for (std::map<t_getJson, iUpmObject*>::const_iterator it = _child_json_deserializers.begin();
                it != _child_json_deserializers.end();)
        {
            // Call static method to serialize data from the derived classes
            ss << it->first(it->second);
            if (++it != _child_json_deserializers.end()) ss << ",";
            ss << std::endl;
        }
        DEBUG_MSG("XXX Deserializers: " << ss.str());
    }

}

std::string iUpmObject::Description()
{
    json x = LibraryJson()["Sensor Class"][Name()]["Description"];

    if (x.empty())
        THROW_RUNTIME("No Description found for sensor class '" + Name() + "' in the JSON library descriptor file: " + LibraryJsonFilename());

    return x;
}

std::string iUpmObject::JsonDefinition ()
{
    std::stringstream ss;
    ss << "{" << std::endl
       << "  \"name\" : \"" << Name() << "\"," << std::endl
       << "  \"description\" : \"" << Description() << "\""
       << std::endl << "}";
    return ss.str();
}

std::string iUpmObject::Name()
{
    /* Make sure the derived class name (from std::type_info) matches a JSON sensor class name */
    std::string classname_derived = DerivedClassName();

    /* Look at all objects under 'Sensor Class' */
    json x = LibraryJson()["Sensor Class"][classname_derived];

    if (x.empty())
        THROW_RUNTIME("Sensor class Name '" + classname_derived + "' was not found in the JSON library descriptor file: " + LibraryJsonFilename());

    return classname_derived;
}

json &iUpmObject::LibraryJson()
{
    /* If no object, create one and load the library descriptor file*/
    if (!_json)
    {
        _json = new json;
        initFromJsonLibDesc(LibraryJsonFilename());
    }

    /* If the library descriptor JSON file has NOT been loaded, load it */
//    if (_json->empty())
//    {
//        initFromJsonLibDesc(LibraryJsonFilename().c_str());
//        //*_json = _json->parse(LibraryJsonRaw());
//    }

    /* Perform a quick error check: Library element must be equal to LibraryBaseName() */
//    std::string lib_base_name = LibraryBaseName();
//    std::string lib_json_name = (*_json)["Library"];
//    if (lib_base_name != (*_json)["Library"])
//        THROW_RUNTIME("Library base name '" + lib_base_name + "' does NOT match the Library name specified in the JSON file '" + lib_json_name +"'");

    return *_json;
}

std::string iUpmObject::DerivedClassName() const
{
    int status = 0;
    char *name_demangled;
    const std::type_info &this_type = typeid(*this);

    /* Demangle class name */
    name_demangled = abi::__cxa_demangle(this_type.name(), 0, 0, &status);
    if (status)
        THROW_RUNTIME("Failed to demangle class name.");

    std::string name = name_demangled;
    free(name_demangled);

    Dl_info info;
    /* Returns 0 on failure */
    if (!dladdr((const void*)&this_type, &info))
        return NULL;

    /* Return class name only */
    return name.substr(name.find_last_of(":") + 1);
}

std::string iUpmObject::LibraryAbsolutePath() const
{
    Dl_info info;
    const std::type_info &this_type = typeid(*this);

    /* Throw on failure */
    if (!dladdr((const void*)&this_type, &info))
        THROW_RUNTIME("Failed to locate the library for this type.");

    /* Attempt a realpath */
    return realpath(info.dli_fname, NULL);
}

std::string iUpmObject::LibraryBaseName() const
{
    /* First, get the full filename */
    std::string filename = LibraryAbsolutePath();

    std::string basename = filename.substr(filename.find_last_of("/\\") + 1);

    /* Regex the library *base* name from the library file name */
    std::smatch sm;
    std::regex reg1("libupm-(\\S+)\\.so");
    if (std::regex_search(basename, sm, reg1))
        basename = sm.str(1);
//    else
//        std::cout << "XXX warning, binary: " << filename << " does not match format: libupm-<libname>.so, using: " << basename;

    //THROW_RUNTIME("Could not get base library name.  UPM library name does not follow the libupm-<library>.so format.");

    return basename;
}

std::string iUpmObject::LibraryJsonFilename() const
{
    if (!_LibraryJsonFilename.empty())
        return _LibraryJsonFilename;

    std::string json_file = DataDirectory() + "/" + LibraryBaseName() + ".json";

    if (!exists(json_file))
        THROW_RUNTIME("Failed to locate library JSON definition: " + json_file);

    return json_file;
}

std::string iUpmObject::DataDirectory() const
{
    std::string lib_loc = LibraryAbsolutePath();

    /* If a json file has already been provided, use it */
    std::string json_lib_def_file = _LibraryJsonFilename;
    /* Otherwise, attempt to build the location of the json file */
    if (json_lib_def_file.empty())
        json_lib_def_file = lib_loc.substr(0, lib_loc.find_last_of("/\\")) + "/" + LibraryBaseName() + ".json";

    /* If no local <libname>.json exists, look in DATADIR */
    if (!exists(json_lib_def_file))
    {
        // If the library is:
        //     /usr/local/lib/libupm-rhusb.so.1.6.0
        // then look one directory up for share/upm like so:
        //     /usr/local/share/upm/rhusb/rhusb.json
        json_lib_def_file = DATA_DIRECTORY_STR + "/upm/" + LibraryBaseName() + "/" + LibraryBaseName() + ".json";
    }

    if (!exists(json_lib_def_file))
        THROW_RUNTIME("Data directory for UPM was not found.  Library found: " + lib_loc + ", expected JSON definition file: " + json_lib_def_file);

    return json_lib_def_file.substr(0, json_lib_def_file.find_last_of("/\\"));
}

std::string iUpmObject::LibraryJsonRaw() const
{
    std::string json_file = LibraryJsonFilename();

    /* Return the contents as a string */
    std::string json_str;
    std::ifstream t(json_file);
    json_str.assign((std::istreambuf_iterator<char>(t)),
            std::istreambuf_iterator<char>());

    return json_str.c_str();
}

std::string iUpmObject::LibraryVersion() const
{
    return ::LibraryVersion();
}

void iUpmObject::AddValueSerializer (iUpmObject * instance, t_getJson method)
{ _child_value_serializers[method] = instance; }

void iUpmObject::AddJsonDeserializer(iUpmObject * instance, t_getJson method)
{ _child_json_deserializers[method] = instance; }
