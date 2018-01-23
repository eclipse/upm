%module javaupm_mma8x5x
%include "../upm.i"

%{
    #include "mma8x5x.hpp"
%}

%include "mma8x5x.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_mma8x5x)