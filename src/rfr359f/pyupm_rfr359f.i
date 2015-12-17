// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_rfr359f
%include "../upm.i"

%feature("autodoc", "3");

%include "rfr359f.h"
%{
    #include "rfr359f.h"
%}
