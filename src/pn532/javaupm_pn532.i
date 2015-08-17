%module javaupm_pn532
%include "../upm.i"
%include "arrays_java.i"

%apply unsigned char[] {uint8_t *};

%{
    #include "pn532.h"
%}

%include "pn532.h"
