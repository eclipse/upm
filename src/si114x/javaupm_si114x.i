%module javaupm_si114x
%include "../upm.i"
%include "cpointer.i"

%{
    #include "si114x.hpp"
%}

%include "si114x.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_si114x)