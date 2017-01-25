// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_uartat
%include "../upm.i"
%include "std_string.i"

%feature("autodoc", "3");

%include "uartat_defs.h"
%include "uartat.hpp"
%{
    #include "uartat.hpp"
%}
