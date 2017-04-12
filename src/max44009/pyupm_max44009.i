%module pyupm_max44009
%include "../upm.i"
// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"


%include "stdint.i"

%feature("autodoc", "3");

%include "max44009.hpp"
%{
    #include "max44009.hpp"
%}
