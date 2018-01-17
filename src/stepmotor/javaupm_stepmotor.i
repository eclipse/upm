%module javaupm_stepmotor
%include "../upm.i"

%{
    #include "stepmotor.hpp"
%}

%include "stepmotor.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_stepmotor)