%module javaupm_mg811
%include "../upm.i"

%{
    #include "mg811.hpp"
%}

%include "mg811.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_mg811)