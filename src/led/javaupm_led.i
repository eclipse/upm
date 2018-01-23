%module javaupm_led
%include "../upm.i"

%apply int {mraa::Edge}

%include "led.hpp"
%{
    #include "led.hpp"
%}

JAVA_JNI_LOADLIBRARY(javaupm_led)
