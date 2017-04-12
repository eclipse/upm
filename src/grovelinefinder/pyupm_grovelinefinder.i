// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_grovelinefinder
%include "../upm.i"

%feature("autodoc", "3");

%include "grovelinefinder.hpp"
%{
    #include "grovelinefinder.hpp"
%}
