%module pyupm_ds1808lc
%include "../upm.i"

%feature("autodoc", "3");

%include "ds1808lc.h"
%{
    #include "ds1808lc.h"
%}
