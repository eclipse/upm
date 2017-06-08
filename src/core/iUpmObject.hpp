#pragma once

#include <map>

#include <sstream>
#include <string>

#include <vector>
#include <stdint.h>
#include <memory>

#if !defined(NDEBUG)
#include <iostream>
#endif

#ifndef SWIG
/* Forward declare the basic_json class */
namespace nlohmann
{
    template<typename, typename>
        struct adl_serializer;

    template<template<typename, typename, typename...> class ObjectType,
        template<typename, typename...> class ArrayType,
        class StringType,
        class BooleanType,
        class NumberIntegerType,
        class NumberUnsignedType,
        class NumberFloatType,
        template<typename> class AllocatorType,
        template<typename, typename> class JSONSerializer>
            class basic_json;

        using json = basic_json<std::map, std::vector, std::string, bool, std::int64_t, std::uint64_t, double, std::allocator, nlohmann::adl_serializer>;
}
#endif

namespace upm
{

inline std::string __func_or_memberfunc(const std::string& cxx_function)
{
    size_t colons = cxx_function.find("::");
    size_t begin = cxx_function.substr(0, colons).rfind(" ") + 1;
    size_t end = cxx_function.rfind(")") - begin + 1;

    return cxx_function.substr(begin, end);
}

#define __PRETTY_FUNCTIONS__ __func_or_memberfunc(__PRETTY_FUNCTION__)

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#if !defined(NDEBUG)
#define DEBUG_MSG(msg) do { std::cout << __FILENAME__ << ":" << __LINE__ \
    << " [" << __PRETTY_FUNCTIONS__ << "] " << msg << std::endl; } while (0)
#else
#define DEBUG_MSG(msg) do {} while (0)
#endif

    class iUpmObject;

    /**
     * Function pointer typedef for child-to-parent proxy call
     */
    typedef std::string (*t_getJson)(iUpmObject*);

    class iUpmObject
    {
        public:
            iUpmObject();
            virtual ~iUpmObject();

            virtual std::string Description();
            virtual std::string JsonDefinition();
            virtual std::string Name();

            virtual void initFromJsonLibDesc(std::string path_to_json_file);
#ifndef SWIG
            nlohmann::json& LibraryJson();
#endif

            std::string DataDirectory() const;
            std::string DerivedClassName() const;
            std::string LibraryAbsolutePath() const;
            std::string LibraryBaseName() const;
            std::string LibraryJsonFilename() const;
            std::string LibraryJsonRaw() const;
            std::string LibraryVersion() const;

        protected:
            void AddValueSerializer(iUpmObject* instance, t_getJson method);
            void AddJsonDeserializer(iUpmObject* instance, t_getJson method);

            /**
             * Used by child classes for child-to-parent proxy call
             */
            std::map<t_getJson, iUpmObject*> _child_value_serializers;

            /**
             * Used by child classes for child-to-parent proxy call
             */
            std::map<t_getJson, iUpmObject*> _child_json_deserializers;

            ///*
            //friend std::ostream& operator<<(std::ostream& os, const iUpmObject& o)
            //{
            //    return os << "  \"name\" : \"" << o.Name() << "\"," << std::endl
            //        << "  \"description\" : \"" << o.Description() << "\"";
            //}

            //friend std::ostream& operator<<(std::ostream& os, const iUpmObject* o)
            //{ return os << *o; }
            //*/
        private:
            nlohmann::json *_json = NULL;
            std::string _Description;
            std::string _LibraryJsonFilename;
            std::string _LibraryJsonRaw;
    };
}
