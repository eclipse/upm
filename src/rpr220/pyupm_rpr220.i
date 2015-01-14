%module pyupm_rpr220
%include "../upm.i"

%feature("autodoc", "3");

%include "rpr220.h"
%{
    #include "rpr220.h"
%}
