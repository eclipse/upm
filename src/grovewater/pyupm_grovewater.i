// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_grovewater
%include "../upm.i"

%feature("autodoc", "3");

%include "grovewater.hpp"
%{
    #include "grovewater.hpp"
%}
