// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_elementzir
%include "../upm.i"

%feature("autodoc", "3");

%include "elementzir.hpp"
%{
    #include "elementzir.hpp"
%}
