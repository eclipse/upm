%module pyupm_lp8860
%include "../upm.i"
// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"

%feature("autodoc", "3");

%include "lp8860.hpp"
%{
    #include "lp8860.hpp"
%}
