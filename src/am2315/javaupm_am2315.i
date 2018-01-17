%module javaupm_am2315
%include "../upm.i"

%{
    #include "am2315.hpp"
%}

%include "am2315.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_am2315)