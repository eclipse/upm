// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_rpr220
%include "../upm.i"

%feature("autodoc", "3");

%include "rpr220.hpp"
%{
    #include "rpr220.hpp"
%}
