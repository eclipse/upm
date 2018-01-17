%module javaupm_ina132
%include "../upm.i"

%{
    #include "ina132.hpp"
%}

%include "ina132.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_ina132)