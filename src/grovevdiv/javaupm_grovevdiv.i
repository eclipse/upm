%module javaupm_grovevdiv
%include "../upm.i"

%{
    #include "grovevdiv.hpp"
%}

%include "grovevdiv.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_grovevdiv)