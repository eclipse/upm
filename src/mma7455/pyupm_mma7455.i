%module pyupm_mma7455
%include "../upm.i"

%feature("autodoc", "3");

%include "mma7455.h"
%{
    #include "mma7455.h"
%}
