%module javaupm_m24lr64e
%include "../upm.i"
%include "stdint.i"
%include "arrays_java.i";

%apply signed char[] {uint8_t *};

%{
    #include "m24lr64e.h"
%}

%include "m24lr64e.h"
