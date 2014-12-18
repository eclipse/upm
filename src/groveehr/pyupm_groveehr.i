%module pyupm_groveehr
%include "../upm.i"

%feature("autodoc", "3");

%include "groveehr.h"
%{
    #include "groveehr.h"
%}
