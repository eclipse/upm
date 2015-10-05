%module(directors="1") javaupm_nrf24l01
%include "../upm.i"

%feature("director") Callback;

%include "arrays_java.i";
%apply signed char[] {uint8_t *};

%include "Callback.h"
%{
    #include "nrf24l01.h"
%}

%include "nrf24l01.h"
