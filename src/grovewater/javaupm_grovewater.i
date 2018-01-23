%module javaupm_grovewater
%include "../upm.i"

%{
    #include "grovewater.hpp"
%}

%include "grovewater.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_grovewater)