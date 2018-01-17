%module javaupm_waterlevel
%include "../upm.i"

%{
    #include "waterlevel.hpp"
%}

%include "waterlevel.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_waterlevel)