%module javaupm_nrf24l01
%include "../upm.i"

%include "arrays_java.i";
%apply unsigned char[] {uint8_t *};

%{
    #include "nrf24l01.h"
%}

%include "nrf24l01.h"
