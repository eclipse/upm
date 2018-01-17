%module javaupm_groveeldriver
%include "../upm.i"

%{
    #include "groveeldriver.hpp"
%}

%include "groveeldriver.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_groveeldriver)