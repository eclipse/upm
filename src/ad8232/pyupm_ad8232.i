%module pyupm_ad8232
%include "../upm.i"

%feature("autodoc", "3");

%include "ad8232.h"
%{
    #include "ad8232.h"
%}
