%module pyupm_hx711
%include "../upm.i"
%include "stdint.i"

%feature("autodoc", "3");

%include "hx711.h"
%{
    #include "hx711.h"
%}
