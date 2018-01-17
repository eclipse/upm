%module javaupm_groveemg
%include "../upm.i"

%{
    #include "groveemg.hpp"
%}

%include "groveemg.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_groveemg)