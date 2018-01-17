%module javaupm_linefinder
%include "../upm.i"

%{
    #include "linefinder.hpp"
%}

%include "linefinder.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_linefinder)