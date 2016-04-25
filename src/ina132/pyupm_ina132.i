// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_ina132
%include "../upm.i"

%feature("autodoc", "3");

%include "ina132.hpp"
%{
    #include "ina132.hpp"
%}
