// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_max44000
%include "../upm.i"

%include "stdint.i"

%feature("autodoc", "3");

%include "max44000.hpp"
%{
    #include "max44000.hpp"
%}
