%module pyupm_mg811
%include "../upm.i"

%feature("autodoc", "3");

%include "mg811.h"
%{
    #include "mg811.h"
%}
