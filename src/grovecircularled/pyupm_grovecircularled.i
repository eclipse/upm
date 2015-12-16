// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_grovecircularled
%include "../upm.i"

%feature("autodoc", "3");

%include "grovecircularled.h"
%{
    #include "grovecircularled.h"
%}
