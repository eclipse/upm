%module pyupm_hlg150h
%include "../upm.i"
// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"

%feature("autodoc", "3");

%include "hlg150h.hpp"
%{
    #include "hlg150h.hpp"
%}
