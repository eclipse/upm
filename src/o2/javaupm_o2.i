%module javaupm_o2
%include "../upm.i"

%{
    #include "o2.hpp"
%}

%include "o2.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_o2)