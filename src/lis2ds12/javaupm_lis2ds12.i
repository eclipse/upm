%module javaupm_lis2ds12
%include "../upm.i"
%include "typemaps.i"
%include "../upm_vectortypes.i"

%ignore getAccelerometer(float *, float *, float *);

%include "lis2ds12_defs.h"
%include "lis2ds12.hpp"
%{
    #include "lis2ds12.hpp"
%}

JAVA_JNI_LOADLIBRARY(javaupm_lis2ds12)
