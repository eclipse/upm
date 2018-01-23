%module javaupm_grovewfs
%include "../upm.i"

%ignore flowISR;

%{
    #include "grovewfs.hpp"
%}

%include "grovewfs.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_grovewfs)