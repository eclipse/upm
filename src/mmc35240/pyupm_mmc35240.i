%module pyupm_mmc35240
// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"

%include "../upm.i"

%feature("autodoc", "3");

%include "mmc35240.hpp"
%{
    #include "mmc35240.hpp"
%}
