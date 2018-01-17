%module javaupm_groveo2
%include "../upm.i"

%{
    #include "groveo2.hpp"
%}

%include "groveo2.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_groveo2)