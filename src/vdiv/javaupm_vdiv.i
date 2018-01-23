%module javaupm_vdiv
%include "../upm.i"

%{
    #include "vdiv.hpp"
%}

%include "vdiv.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_vdiv)