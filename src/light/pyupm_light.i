// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_light
%include "../upm.i"

%feature("autodoc", "3");

%include "light.hpp"
%{
    #include "light.hpp"
%}
