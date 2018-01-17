%module javaupm_mcp9808
%include "../upm.i"

%{
    #include "mcp9808.hpp"
%}

%include "mcp9808.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_mcp9808)