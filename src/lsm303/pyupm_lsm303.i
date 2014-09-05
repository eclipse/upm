%module pyupm_lsm303
%include "../upm.i"

%feature("autodoc", "3");

%include "lsm303.h"
%{
    #include "lsm303.h"
%}
