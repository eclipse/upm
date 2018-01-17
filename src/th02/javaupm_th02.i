%module javaupm_th02
%include "../upm.i"

%{
    #include "th02.hpp"
%}

%include "th02.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_th02)