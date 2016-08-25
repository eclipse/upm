// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_grovecollision
%include "../upm.i"

%feature("autodoc", "3");

%include "grovecollision.hpp"
%{
    #include "grovecollision.hpp"
%}
