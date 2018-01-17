%module javaupm_lpd8806
%include "../upm.i"

%{
    #include "lpd8806.hpp"
%}

%include "lpd8806.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_lpd8806)