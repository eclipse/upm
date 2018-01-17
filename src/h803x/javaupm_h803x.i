%module javaupm_h803x
%include "../upm.i"
%include "typemaps.i"

%{
    #include "h803x.hpp"
%}

%include "h803x.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_h803x)
