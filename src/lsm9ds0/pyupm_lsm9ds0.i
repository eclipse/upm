%module pyupm_lsm9ds0
%include "../upm.i"
%include "cpointer.i"

%include "stdint.i"

%feature("autodoc", "3");

%pointer_functions(float, floatp);

%include "lsm9ds0.h"
%{
    #include "lsm9ds0.h"
%}

