%module javaupm_veml6070
%include "../upm.i"
%include "stdint.i"
%include "typemaps.i"

%{
    #include "veml6070.hpp"
%}
%include "veml6070.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_veml6070)
