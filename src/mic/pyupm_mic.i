%module pyupm_mic
%include "../upm.i"
%include "../carrays_uint16_t.i"

%include "stdint.i"

%feature("autodoc", "3");

%include "mic.h"
%{
    #include "mic.h"
%}
