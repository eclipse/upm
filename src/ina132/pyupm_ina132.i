// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_ina132
%include "../upm.i"

%feature("autodoc", "3");

%include "ina132.h"
%{
    #include "ina132.h"
%}
