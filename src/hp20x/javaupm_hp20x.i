%module javaupm_hp20x
%include "../upm.i"

%{
    #include "hp20x.hpp"
%}

%include "hp20x.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_hp20x)