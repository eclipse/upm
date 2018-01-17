%module javaupm_abp
%include "../upm.i"
%include "stdint.i"
%include "typemaps.i"

%{
    #include "abp.hpp"
%}
%include "abp.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_abp)
