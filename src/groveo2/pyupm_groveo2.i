// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_groveo2
%include "../upm.i"

%feature("autodoc", "3");

%include "groveo2.hpp"
%{
    #include "groveo2.hpp"
%}
