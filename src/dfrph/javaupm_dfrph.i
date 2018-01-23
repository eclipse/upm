%module javaupm_dfrph
%include "../upm.i"

%{
    #include "dfrph.hpp"
%}

%include "dfrph.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_dfrph)