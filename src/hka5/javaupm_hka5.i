%module javaupm_hka5
%include "../upm.i"
%include "std_string.i"
%include "stdint.i"
%include "typemaps.i"

%include "hka5.hpp"
%{
    #include "hka5.hpp"
%}

JAVA_JNI_LOADLIBRARY(javaupm_hka5)
