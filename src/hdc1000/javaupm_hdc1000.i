%module javaupm_hdc1000
%include "../upm.i"

%{
    #include "hdc1000.hpp"
%}

%include "hdc1000.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_hdc1000)