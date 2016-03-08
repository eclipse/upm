// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_mics6814
%include "../upm.i"

%feature("autodoc", "3");

%include "mics6814.h"
%{
    #include "mics6814.h"
%}
