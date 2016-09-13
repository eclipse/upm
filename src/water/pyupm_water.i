// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_water
%include "../upm.i"

%feature("autodoc", "3");

%include "water.hpp"
%{
    #include "water.hpp"
%}
