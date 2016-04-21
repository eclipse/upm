%module pyupm_mmc35240
%include "../upm.i"

%feature("autodoc", "3");

%include "mmc35240.h"
%{
    #include "mmc35240.h"
%}
