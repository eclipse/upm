%module javaupm_mpu9150
%include "../upm.i"
%include "typemaps.i"

%apply float *INOUT { float *x, float *y, float *z  };

%{
    #include "mpu9150.h"
%}

%include "mpu9150.h"
