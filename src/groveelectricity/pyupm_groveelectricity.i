%module pyupm_groveelectricity
%include "../upm.i"

%feature("autodoc", "3");

%include "groveelectricity.h"
%{
    #include "groveelectricity.h"
%}
