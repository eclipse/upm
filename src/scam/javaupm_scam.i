%module javaupm_scam
%include "../upm.i"
%include "../java_buffer.i"

%{
    #include "scam.hpp"
%}

%include "scam.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_scam)
