%module javaupm_ecs1030
%include "../upm.i"

%{
    #include "ecs1030.hpp"
%}

%include "ecs1030.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_ecs1030)