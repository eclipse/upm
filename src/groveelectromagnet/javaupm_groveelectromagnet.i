%module javaupm_groveelectromagnet
%include "../upm.i"

%{
    #include "groveelectromagnet.hpp"
%}

%include "groveelectromagnet.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_groveelectromagnet)