%module pyupm_ozw
%include "../upm.i"
%include "cpointer.i"

%include "stdint.i"

%feature("autodoc", "3");

%pointer_functions(float, floatp);

%include "ozw.h"
%{
    #include "ozw.h"
%}

