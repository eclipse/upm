%module javaupm_rsc
%include "../upm.i"

%{
    #include "rsc.hpp"
%}

%include "rsc.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_rsc)
