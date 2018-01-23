%module javaupm_md
%include "../upm.i"

%{
    #include "md.hpp"
%}

%include "md_defs.h"
%include "md.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_md)
