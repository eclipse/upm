// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_uln200xa
%include "../upm.i"

%feature("autodoc", "3");

%include "uln200xa.hpp"
%{
    #include "uln200xa.hpp"
%}
