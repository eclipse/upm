// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_grovescam
%include "../upm.i"

%feature("autodoc", "3");

%{
    #include "grovescam.hpp"
%}
%include "grovescam.hpp"
