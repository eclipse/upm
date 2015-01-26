%module pyupm_adxl335
%include "../upm.i"

%feature("autodoc", "3");

%include "adxl335.h"
%{
    #include "adxl335.h"
%}
