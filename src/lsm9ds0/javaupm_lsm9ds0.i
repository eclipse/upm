%module javaupm_lsm9ds0
%include "../upm.i"
%include "cpointer.i"
%include "typemaps.i"

%apply float *INOUT { float *x, float *y, float *z };

%include "lsm9ds0.h"
%{
    #include "lsm9ds0.h"
%}
%include "lsm9ds0.h"
