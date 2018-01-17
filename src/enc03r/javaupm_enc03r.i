%module javaupm_enc03r
%include "../upm.i"

%{
    #include "enc03r.hpp"
%}

%include "enc03r.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_enc03r)