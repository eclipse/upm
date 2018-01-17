%module javaupm_grovelinefinder
%include "../upm.i"

%{
    #include "grovelinefinder.hpp"
%}

%include "grovelinefinder.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_grovelinefinder)