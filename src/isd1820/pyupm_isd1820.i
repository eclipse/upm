%module pyupm_isd1820
%include "../upm.i"

%feature("autodoc", "3");

%{
    #include "isd1820.h"
%}

%include "isd1820.h"

