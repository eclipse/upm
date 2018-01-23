%module javaupm_ht9170
%include "../upm.i"

%{
    #include "ht9170.hpp"
%}

%include "ht9170.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_ht9170)