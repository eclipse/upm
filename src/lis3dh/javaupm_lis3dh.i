%module javaupm_lis3dh
%include "../upm.i"
%include "typemaps.i"
%include "../upm_vectortypes.i"

%ignore getAccelerometer(float *, float *, float *);

%include "lis3dh_defs.h"
%include "lis3dh.hpp"
%{
    #include "lis3dh.hpp"
%}

JAVA_JNI_LOADLIBRARY(javaupm_lis3dh)
