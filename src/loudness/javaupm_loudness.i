%module javaupm_loudness
%include "../upm.i"

%{
    #include "loudness.hpp"
%}

%include "loudness.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_loudness)