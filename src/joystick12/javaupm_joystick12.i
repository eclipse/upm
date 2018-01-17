%module javaupm_joystick12
%include "../upm.i"

%{
    #include "joystick12.hpp"
%}

%include "joystick12.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_joystick12)