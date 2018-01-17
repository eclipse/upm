%module javaupm_l298
%include "../upm.i"

%{
    #include "l298.hpp"
%}

%include "l298.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_l298)