%module javaupm_adafruitss
%include "../upm.i"

%include "adafruitss.hpp"
%{
    #include "adafruitss.hpp"
%}

JAVA_JNI_LOADLIBRARY(javaupm_adafruitss)