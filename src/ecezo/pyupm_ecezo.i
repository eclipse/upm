// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_ecezo
%include "../upm.i"
%include "std_string.i"

%feature("autodoc", "3");

%include "ecezo_defs.h"
%include "ecezo.hpp"
%{
    #include "ecezo.hpp"
%}
