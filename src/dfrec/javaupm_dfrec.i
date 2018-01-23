%module javaupm_dfrec
%include "../upm.i"
%include "std_string.i"
%include "cpointer.i"
%include "typemaps.i"

%include "dfrec.hpp"
%{
    #include "dfrec.hpp"
%}

JAVA_JNI_LOADLIBRARY(javaupm_dfrec)
