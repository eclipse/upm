%module javaupm_ultrasonic
%include "../upm.i"

%ignore signalISR;

%{
    #include "ultrasonic.hpp"
%}

%include "ultrasonic.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_ultrasonic)
