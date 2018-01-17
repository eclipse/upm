%module javaupm_water
%include "../upm.i"

%{
    #include "water.hpp"
%}

%include "water.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_water)