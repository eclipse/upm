%module javaupm_smartdrive
%include "../upm.i"

%{
    #include "smartdrive.hpp"
%}

%include "smartdrive.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_smartdrive)
