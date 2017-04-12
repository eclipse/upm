// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_micsv89
%include "../upm.i"

%feature("autodoc", "3");

%include "micsv89.hpp"
%{
    #include "micsv89.hpp"
%}
