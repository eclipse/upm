%module javaupm_max44000
%include "../upm.i"

%{
    #include "max44000.hpp"
%}

%include "max44000.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_max44000)