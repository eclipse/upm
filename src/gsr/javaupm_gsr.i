%module javaupm_gsr
%include "../upm.i"

%{
    #include "gsr.hpp"
%}

%include "gsr.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_gsr)