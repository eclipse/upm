// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_groveemg
%include "../upm.i"

%feature("autodoc", "3");

%include "groveemg.hpp"
%{
    #include "groveemg.hpp"
%}
