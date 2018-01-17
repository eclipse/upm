%module javaupm_flex
%include "../upm.i"

%{
    #include "flex.hpp"
%}

%include "flex.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_flex)