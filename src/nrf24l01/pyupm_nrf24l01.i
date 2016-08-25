// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_nrf24l01
%include "../upm.i"
%include "../carrays_uint8_t.i"

%feature("autodoc", "3");

%include "nrf24l01.hpp"
%{
    #include "nrf24l01.hpp"
%}
