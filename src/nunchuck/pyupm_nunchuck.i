%module pyupm_nunchuck
%include "../upm.i"

%feature("autodoc", "3");

%include "nunchuck.h"
%{
    #include "nunchuck.h"
%}
