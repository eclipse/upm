%module javaupm_nunchuck
%include "../upm.i"
%include "../java_buffer.i"

%{
    #include "nunchuck.hpp"
%}

%include "nunchuck.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_nunchuck)