%module pyupm_l298
%include "../upm.i"

%feature("autodoc", "3");

%include "l298.h"
%{
    #include "l298.h"
%}
