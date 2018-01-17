%module javaupm_tcs3414cs
%include "../upm.i"

%{
    #include "tcs3414cs.hpp"
%}

%include "tcs3414cs.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_tcs3414cs)