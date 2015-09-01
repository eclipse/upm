%module javaupm_mpr121
%include "../upm.i"
%include "arrays_java.i";

%apply signed char[] {uint8_t *};

%{
    #include "mpr121.h"
%}

%include "mpr121.h"
