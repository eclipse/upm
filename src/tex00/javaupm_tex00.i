%module javaupm_tex00
%include "../upm.i"
%include "std_string.i"

%include "tex00.hpp"
%{
    #include "tex00.hpp"
%}

JAVA_JNI_LOADLIBRARY(javaupm_tex00)
