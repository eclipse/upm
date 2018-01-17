%module javaupm_adis16448
%include "../upm.i"

%{
    #include "adis16448.hpp"
%}

%include "adis16448.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_adis16448)