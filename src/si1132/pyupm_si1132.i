%module pyupm_si1132
%include "../upm.i"

%include "stdint.i"

%feature("autodoc", "3");

%include "si1132.h"
%{
    #include "si1132.h"
%}
