// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_rotary
%include "../upm.i"

%feature("autodoc", "3");

%include "rotary.hpp"
%{
    #include "rotary.hpp"
%}
