%module javaupm_guvas12d
%include "../upm.i"

%{
    #include "guvas12d.hpp"
%}

%include "guvas12d.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_guvas12d)