// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_nrf24l01
%include "../upm.i"

%feature("autodoc", "3");

%include "nrf24l01.h"
%{
    #include "nrf24l01.h"
%}
