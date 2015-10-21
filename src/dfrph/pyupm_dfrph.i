%module pyupm_dfrph
%include "../upm.i"

%feature("autodoc", "3");

%include "dfrph.h"
%{
    #include "dfrph.h"
%}
