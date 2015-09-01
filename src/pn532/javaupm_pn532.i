%module javaupm_pn532
%include "../upm.i"
%include "typemaps.i"
%include "arrays_java.i"

%apply signed char[] {uint8_t *};
%apply unsigned char *INOUT {uint8_t *responseLength};
%apply unsigned char *OUTPUT {uint8_t *uidLength};

%ignore i2cContext;

%{
    #include "pn532.h"
%}

%include "pn532.h"
