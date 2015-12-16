// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_adafruitss
%include "../upm.i"

%feature("autodoc", "3");

%include "adafruitss.h"
%{
    #include "adafruitss.h"
%}
