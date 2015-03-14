%module pyupm_groveelectromagnet
%include "../upm.i"

%feature("autodoc", "3");

%include "groveelectromagnet.h"
%{
    #include "groveelectromagnet.h"
%}
