%module pyupm_wt5001
%include "../upm.i"

%feature("autodoc", "3");

%include "wt5001.h"
%{
    #include "wt5001.h"
%}
