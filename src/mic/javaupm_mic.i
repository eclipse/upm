%module javaupm_mic
%include "../upm.i"
%include "stdint.i"
%include "typemaps.i"

%apply uint16_t *OUTPUT { uint16_t *buffer };

%{
    #include "mic.h"
%}

%include "mic.h"
