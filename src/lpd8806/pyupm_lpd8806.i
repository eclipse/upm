// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_lpd8806
%include "../upm.i"

%feature("autodoc", "3");

%include "lpd8806.hpp"
%{
    #include "lpd8806.hpp"
%}
