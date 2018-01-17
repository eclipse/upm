%module javaupm_collision
%include "../upm.i"

%{
    #include "collision.hpp"
%}

%include "collision.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_collision)