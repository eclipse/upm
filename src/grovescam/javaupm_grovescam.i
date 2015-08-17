%module javaupm_grovescam
%include "../upm.i"
%include "arrays_java.i";
%apply unsigned char[] {uint8_t *};

%{
    #include "grovescam.h"
%}

%include "grovescam.h"
