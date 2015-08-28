%module javaupm_mic
%include "../upm.i"
%include "stdint.i"
%include "arrays_java.i"

%apply unsigned short[] {uint16_t *buffer}; 

%{
    #include "mic.h"
%}

%include "mic.h"
