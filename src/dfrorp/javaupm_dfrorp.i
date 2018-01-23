%module javaupm_dfrorp
%include "../upm.i"
%include "std_string.i"
%include "cpointer.i"
%include "typemaps.i"

%include "dfrorp.hpp"
%{
    #include "dfrorp.hpp"
%}

JAVA_JNI_LOADLIBRARY(javaupm_dfrorp)
