%module javaupm_lsm9ds0
%include "../upm.i"
%include "cpointer.i"
%include "typemaps.i"
%include "arrays_java.i";

%apply float *INOUT { float *x, float *y, float *z };
%apply unsigned char[] {uint8_t *buf};

%include "lsm9ds0.h"
%{
    #include "lsm9ds0.h"
%}
%include "lsm9ds0.h"
