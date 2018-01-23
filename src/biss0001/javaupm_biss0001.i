%module javaupm_biss0001
%include "../upm.i"

%{
    #include "biss0001.hpp"
%}

%include "biss0001.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_biss0001)