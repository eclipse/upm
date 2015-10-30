%module pyupm_sx1276
%include "../upm.i"
%include "cpointer.i"

%include "stdint.i"

%feature("autodoc", "3");

%pointer_functions(float, floatp);

%include "sx1276.h"
%{
    #include "sx1276.h"
%}

