// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_guvas12d
%include "../upm.i"

%feature("autodoc", "3");

%include "guvas12d.hpp"
%{
    #include "guvas12d.hpp"
%}
