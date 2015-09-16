%module javaupm_grovewfs
%include "../upm.i"

%ignore flowISR;

%{
    #include "grovewfs.h"
%}

%include "grovewfs.h"
