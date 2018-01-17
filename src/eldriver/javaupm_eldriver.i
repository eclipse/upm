%module javaupm_eldriver
%include "../upm.i"

%{
    #include "eldriver.hpp"
%}

%include "eldriver.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_eldriver)