// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_th02
%include "../upm.i"

%feature("autodoc", "3");

%include "th02.h"
%{
    #include "th02.h"
%}
