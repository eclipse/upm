%module javaupm_rotaryencoder
%include "../upm.i"

%ignore signalAISR;

%{
    #include "rotaryencoder.hpp"
%}

%include "rotaryencoder.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_rotaryencoder)