%module javaupm_moisture
%include "../upm.i"

%{
    #include "moisture.hpp"
%}

%include "moisture.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_moisture)