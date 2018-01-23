%module javaupm_rn2903
%include "../upm.i"
%include "std_string.i"
%include "stdint.i"
%include "typemaps.i"

%include "rn2903_defs.h"
%include "rn2903.hpp"
%{
    #include "rn2903.hpp"
%}

JAVA_JNI_LOADLIBRARY(javaupm_rn2903)
