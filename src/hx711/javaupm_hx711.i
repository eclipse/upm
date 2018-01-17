%module javaupm_hx711
%include "../upm.i"

%{
    #include "hx711.hpp"
%}

%include "hx711.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_hx711)