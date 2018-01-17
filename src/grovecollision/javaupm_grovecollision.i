%module javaupm_grovecollision
%include "../upm.i"

%{
    #include "grovecollision.hpp"
%}

%include "grovecollision.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_grovecollision)