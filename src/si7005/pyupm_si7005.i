%module pyupm_si7005
%include "../upm.i"

%include "stdint.i"

%feature("autodoc", "3");

%include "si7005.h"
%{
    #include "si7005.h"
%}
