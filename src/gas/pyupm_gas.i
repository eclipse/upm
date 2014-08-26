%module pyupm_gas
%include "../upm.i"
%include "../carrays_uint16_t.i"

%include "stdint.i"

%feature("autodoc", "3");

%include "gas.h"
%{
    #include "gas.h"
%}
