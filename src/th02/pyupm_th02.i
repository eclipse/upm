%module pyupm_th02
%include "../upm.i"

%feature("autodoc", "3");

%include "th02.h"
%{
    #include "th02.h"
%}
