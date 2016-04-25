// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_rhusb
%include "../upm.i"
%include "stdint.i"

%feature("autodoc", "3");

%include "rhusb.hpp"
%{
    #include "rhusb.hpp"
%}
