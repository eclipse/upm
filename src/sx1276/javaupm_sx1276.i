%module(directors="1") javaupm_sx1276
%include "../upm.i"
%include "cpointer.i"
%include "typemaps.i"
%include "arrays_java.i";
%include "../java_buffer.i"

%{
    #include "sx1276.h"
%}

%ignore send(uint8_t *buffer, uint8_t size, int txTimeout);
%ignore getRxBuffer();

%include "sx1276.h"
