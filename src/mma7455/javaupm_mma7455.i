%module javaupm_mma7455
%include "../upm.i"
%include "typemaps.i"
%include "arrays_java.i";

%apply unsigned char[] {unsigned char *};
%apply short *OUTPUT { short * ptrX, short * ptrY, short * ptrZ };

%{
    #include "mma7455.h"
%}

%include "mma7455.h"
