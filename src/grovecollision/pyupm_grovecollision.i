// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_grovecollision
%include "../upm.i"

%feature("autodoc", "3");

%include "grovecollision.h"
%{
    #include "grovecollision.h"
%}
