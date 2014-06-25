%module pyupm_gy65
%include "../upm.i"

%include "stdint.i"

%feature("autodoc", "3");

%include "gy65.h"
%{
    #include "gy65.h"
%}
