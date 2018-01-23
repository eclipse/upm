%module javaupm_maxsonarez
%include "../upm.i"

%{
    #include "maxsonarez.hpp"
%}

%include "maxsonarez.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_maxsonarez)