%module pyupm_lpd8806
%include "../upm.i"

%feature("autodoc", "3");

%include "lpd8806.h"
%{
    #include "lpd8806.h"
%}
