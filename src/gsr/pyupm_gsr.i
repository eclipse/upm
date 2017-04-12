// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_gsr
%include "../upm.i"

%feature("autodoc", "3");

%include "gsr.hpp"
%{
    #include "gsr.hpp"
%}
