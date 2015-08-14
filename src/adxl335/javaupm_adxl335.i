%module javaupm_adxl335
%include "../upm.i"
%include "cpointer.i"
%include "typemaps.i"

%apply int *OUTPUT { int *xVal, int *yVal, int *zVal };
%apply float *OUTPUT { float *xAccel, float *yAccel, float *zAccel  };

%{
    #include "adxl335.h"
%}

%include "adxl335.h"
