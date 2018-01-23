%module javaupm_groveultrasonic
%include "../upm.i"

%ignore signalISR;

%{
    #include "groveultrasonic.hpp"
%}

%include "groveultrasonic.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_groveultrasonic)
