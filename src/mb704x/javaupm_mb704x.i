%module javaupm_mb704x
%include "../upm.i"
%include "std_string.i"
%include "stdint.i"
%include "typemaps.i"

%include "mb704x.hpp"
%{
    #include "mb704x.hpp"
%}

JAVA_JNI_LOADLIBRARY(javaupm_mb704x)
