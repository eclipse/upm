%module javaupm_ad8232
%include "../upm.i"

%{
    #include "ad8232.hpp"
%}

%include "ad8232.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_ad8232)