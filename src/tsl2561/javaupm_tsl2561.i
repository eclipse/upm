%module javaupm_tsl2561
%include "../upm.i"

%{
    #include "tsl2561.hpp"
%}

%include "tsl2561.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_tsl2561)