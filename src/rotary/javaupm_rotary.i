%module javaupm_rotary
%include "../upm.i"

%{
    #include "rotary.hpp"
%}

%include "rotary.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_rotary)
