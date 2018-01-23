%module javaupm_grovegsr
%include "../upm.i"

%{
    #include "grovegsr.hpp"
%}

%include "grovegsr.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_grovegsr)