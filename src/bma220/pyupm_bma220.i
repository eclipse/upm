%module pyupm_bma220
%include "../upm.i"
%include "cpointer.i"

%include "stdint.i"

%feature("autodoc", "3");

%pointer_functions(float, floatp);

%include "bma220.h"
%{
    #include "bma220.h"
%}

