%module javaupm_sht1x
%include "../upm.i"
%include "std_string.i"
%include "stdint.i"
%include "typemaps.i"

%include "sht1x_defs.h"
%include "sht1x.hpp"
%{
    #include "sht1x.hpp"
%}

JAVA_JNI_LOADLIBRARY(javaupm_sht1x)
