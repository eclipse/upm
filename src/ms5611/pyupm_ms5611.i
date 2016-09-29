%module pyupm_ms5611
%include "../upm.i"
// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"

%feature("autodoc", "3");

%include "ms5611.hpp"
%{
    #include "ms5611.hpp"
%}
