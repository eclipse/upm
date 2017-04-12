// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_nunchuck
%include "../upm.i"

%feature("autodoc", "3");

%include "nunchuck.hpp"
%{
    #include "nunchuck.hpp"
%}
