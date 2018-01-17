%module javaupm_lsm6ds3h
%include "../upm.i"
%include "typemaps.i"
%include "../upm_vectortypes.i"

%ignore getAccelerometer(float *, float *, float *);
%ignore getGyroscope(float *, float *, float *);

%include "lsm6ds3h_defs.h"
%include "lsm6ds3h.hpp"
%{
    #include "lsm6ds3h.hpp"
%}

JAVA_JNI_LOADLIBRARY(javaupm_lsm6ds3h)
