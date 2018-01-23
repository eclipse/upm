%module javaupm_hwxpxx
%include "../upm.i"
%include "typemaps.i"

%{
    #include "hwxpxx.hpp"
%}

%include "hwxpxx.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_hwxpxx)
