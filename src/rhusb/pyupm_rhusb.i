// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_rhusb
%include "../upm.i"
%include "stdint.i"

%include "rhusb.hpp"
%{
    #include "rhusb.hpp"
%}
