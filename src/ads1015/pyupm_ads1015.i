%module pyupm_ads1015
%include "../upm.i"
%include "stdint.i"

%feature("autodoc", "3");

%include "ads1015.h"
%{
    #include "ads1015.h"
%}
