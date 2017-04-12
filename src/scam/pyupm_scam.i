// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_scam
%include "../upm.i"

%feature("autodoc", "3");

%{
    #include "scam.hpp"
%}
%include "scam.hpp"
