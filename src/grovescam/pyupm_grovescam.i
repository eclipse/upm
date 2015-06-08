%module pyupm_grovescam
%include "../upm.i"

%feature("autodoc", "3");

%{
    #include "grovescam.h"
%}
%include "grovescam.h"
