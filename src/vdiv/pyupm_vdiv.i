// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_vdiv
%include "../upm.i"

%feature("autodoc", "3");

%include "vdiv.hpp"
%{
    #include "vdiv.hpp"
%}
