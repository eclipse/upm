%module javaupm_grovemd
%include "../upm.i"

%{
    #include "grovemd.hpp"
%}

%include "grovemd.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_grovemd)