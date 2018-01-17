%module javaupm_lidarlitev3
%include "../upm.i"

%{
    #include "lidarlitev3.hpp"
%}

%include "lidarlitev3.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_lidarlitev3)
