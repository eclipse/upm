%module pyupm_si1132
%include "../upm.i"
// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"


%include "stdint.i"

%feature("autodoc", "3");

%include "si1132.hpp"
%{
    #include "si1132.hpp"
%}
