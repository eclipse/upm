%module pyupm_flex
%include "../upm.i"

%feature("autodoc", "3");

%include "flex.h"
%{
    #include "flex.h"
%}
