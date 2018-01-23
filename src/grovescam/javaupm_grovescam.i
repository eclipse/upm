%module javaupm_grovescam
%include "../upm.i"
%include "../java_buffer.i"

%{
    #include "grovescam.hpp"
%}

%include "grovescam.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_grovescam)