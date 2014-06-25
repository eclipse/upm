%module pyupm_nrf24l01
%include "../upm.i"

%feature("autodoc", "3");

%include "nrf24l01.h"
%{
    #include "nrf24l01.h"
%}
