%module pyupm_nrf8001
%include "../upm.i"

%feature("autodoc", "3");

%include "nrf8001.h"
%{
    #include "nrf8001.h"
%}
