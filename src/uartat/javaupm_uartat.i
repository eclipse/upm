%module javaupm_uartat
%include "../upm.i"
%include "std_string.i"
%include "stdint.i"
%include "typemaps.i"

%include "uartat_defs.h"
%include "uartat.hpp"
%{
    #include "uartat.hpp"
%}

JAVA_JNI_LOADLIBRARY(javaupm_uartat)
