%module javaupm_max31723
%include "../upm.i"

%{
    #include "max31723.hpp"
%}

%include "max31723.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_max31723)