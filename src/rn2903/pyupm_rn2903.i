// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_rn2903
%include "../upm.i"
%include "std_string.i"

%feature("autodoc", "3");

%include "rn2903_defs.h"
%include "rn2903.hpp"
%{
    #include "rn2903.hpp"
%}
