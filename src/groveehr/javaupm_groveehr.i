%module javaupm_groveehr
%include "../upm.i"

%ignore beatISR;

%{
    #include "groveehr.hpp"
%}

%include "groveehr.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_groveehr)