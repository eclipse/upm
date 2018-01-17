%module javaupm_sm130
%include "../upm.i"

%{
    #include "sm130.hpp"
%}

%include "sm130.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_sm130)