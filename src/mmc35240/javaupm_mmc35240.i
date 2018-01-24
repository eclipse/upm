%module javaupm_mmc35240
%include "../upm.i"

%{
    #include "mmc35240.hpp"
%}

%include "mmc35240.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_mmc35240)
