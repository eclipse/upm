%module javaupm_max5487
%include "../upm.i"

%{
    #include "max5487.hpp"
%}

%include "max5487.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_max5487)