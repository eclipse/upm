%module javaupm_apds9002
%include "../upm.i"

%{
    #include "apds9002.hpp"
%}

%include "apds9002.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_apds9002)