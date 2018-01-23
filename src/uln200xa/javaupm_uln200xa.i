%module javaupm_uln200xa
%include "../upm.i"

%include "uln200xa_defs.h"
%include "uln200xa.hpp"
%{
    #include "uln200xa.hpp"
%}


JAVA_JNI_LOADLIBRARY(javaupm_uln200xa)
