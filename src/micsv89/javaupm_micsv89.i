%module javaupm_micsv89
%include "../upm.i"

%{
    #include "micsv89.hpp"
%}

%include "micsv89.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_micsv89)