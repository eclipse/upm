// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_groveehr
%include "../upm.i"

%feature("autodoc", "3");

%include "groveehr.hpp"
%{
    #include "groveehr.hpp"
%}
