%module javaupm_ta12200
%include "../upm.i"

%{
    #include "ta12200.hpp"
%}

%include "ta12200.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_ta12200)