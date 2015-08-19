%module javaupm_mma7660
%include "../upm.i"
%include "cpointer.i"
%include "typemaps.i"

%feature("director") IsrCallback;

%ignore generic_callback_isr;
%include "../IsrCallback.h"

%apply int *OUTPUT { int *x, int *y, int *z };
%apply float *OUTPUT { float *ax, float *ay, float *az  };

%{
    #include "mma7660.h"
%}

%include "mma7660.h"
