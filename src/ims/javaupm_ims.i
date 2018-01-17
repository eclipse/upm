%module javaupm_ims
%include "../upm.i"

%{
    #include "ims.hpp"
%}

%include "ims_defs.h"
%include "ims.hpp"

JAVA_JNI_LOADLIBRARY(javaupm_ims)
