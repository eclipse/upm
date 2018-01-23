%module javaupm_ecezo
%include "../upm.i"
%include "std_string.i"
%include "stdint.i"
%include "typemaps.i"

%include "ecezo_defs.h"
%include "ecezo.hpp"
%{
    #include "ecezo.hpp"
%}

JAVA_JNI_LOADLIBRARY(javaupm_ecezo)
