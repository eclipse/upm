%module javaupm_adxrs610
%include "../upm.i"

%{
    #include "adxrs610.hpp"
%}

%include "adxrs610.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_adxrs610)