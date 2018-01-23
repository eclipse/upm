%module javaupm_pca9685
%include "../upm.i"
%include "cpointer.i"

%{
    #include "pca9685.hpp"
%}

%include "pca9685.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_pca9685)