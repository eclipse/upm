%module javaupm_zfm20
%include "../upm.i"
%include "stdint.i"
%include "typemaps.i"
%include "arrays_java.i";

%apply unsigned char[] { unsigned char *pkt };
%apply signed char[] { char *buffer };

%apply uint16_t *OUTPUT { uint16_t *id, uint16_t *score };


%{
    #include "zfm20.h"
    speed_t int_B57600 = B57600;
%}

%include "zfm20.h"
speed_t int_B57600 = B57600;
