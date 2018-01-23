%module javaupm_light
%include "../upm.i"

%{
    #include "light.hpp"
%}

%include "light.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_light)