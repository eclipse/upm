%module javaupm_l3gd20
%include "../upm.i"

%{
    #include "l3gd20.hpp"
%}

%include "l3gd20.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_l3gd20)
