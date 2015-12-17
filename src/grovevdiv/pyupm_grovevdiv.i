// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_grovevdiv
%include "../upm.i"

%feature("autodoc", "3");

%include "grovevdiv.h"
%{
    #include "grovevdiv.h"
%}
