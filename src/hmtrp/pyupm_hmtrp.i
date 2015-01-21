%module pyupm_hmtrp
%include "../upm.i"

%feature("autodoc", "3");

%include "hmtrp.h"
%{
    #include "hmtrp.h"
%}
