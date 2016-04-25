// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_gp2y0a
%include "../upm.i"

%feature("autodoc", "3");

%include "gp2y0a.hpp"
%{
    #include "gp2y0a.hpp"
%}
