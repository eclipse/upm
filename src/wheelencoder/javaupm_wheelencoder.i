%module javaupm_wheelencoder
%include "../upm.i"

%{
    #include "wheelencoder.hpp"
%}

%include "wheelencoder.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_wheelencoder)