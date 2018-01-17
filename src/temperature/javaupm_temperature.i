%module (docstring="Temperature sensor") javaupm_temperature

%include "../upm.i"

%include "temperature.hpp"
%{
    #include "temperature.hpp"
%}

JAVA_JNI_LOADLIBRARY(javaupm_temperature)
