%module javaupm_zfm20
%include "../upm.i"
%include "cpointer.i"
%include "stdint.i"
%include "typemaps.i"

%apply uint16_t *OUTPUT { uint16_t *id, uint16_t *score };
%apply unsigned char *OUTPUT { unsigned char *pkt};

/* Send "int *" to Java as intp */
%pointer_functions(int, intp);
%{
    #include "zfm20.h"
    speed_t int_B57600 = B57600;
%}

%include "zfm20.h"
speed_t int_B57600 = B57600;
