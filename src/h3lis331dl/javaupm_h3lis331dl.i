%module javaupm_h3lis331dl
%include "../upm.i"
%include "cpointer.i"
%include "typemaps.i"

%apply int *OUTPUT { int *x, int *y, int*z };
%apply float *OUTPUT { float *aX, float *aY, float *aZ  };


%{
    #include "h3lis331dl.h"
%}

%include "h3lis331dl.h"
