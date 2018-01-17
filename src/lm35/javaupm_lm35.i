%module javaupm_lm35
%include "../upm.i"

%{
    #include "lm35.hpp"
%}

%include "lm35.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_lm35)