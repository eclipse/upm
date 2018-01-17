%module javaupm_wfs
%include "../upm.i"

%ignore flowISR;

%{
    #include "wfs.hpp"
%}

%include "wfs.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_wfs)
