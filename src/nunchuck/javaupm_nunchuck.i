%module javaupm_nunchuck
%include "../upm.i"
%include "arrays_java.i";

%apply unsigned char[] {uint8_t *};

%{
    #include "nunchuck.h"
%}

%include "nunchuck.h"
