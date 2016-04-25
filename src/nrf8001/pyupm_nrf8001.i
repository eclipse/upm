// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_nrf8001
%include "../upm.i"

%feature("autodoc", "3");

%include "nrf8001.hpp"
%{
    #include "nrf8001.hpp"
%}
