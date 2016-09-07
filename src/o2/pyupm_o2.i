// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_o2
%include "../upm.i"

%feature("autodoc", "3");

%include "o2.hpp"
%{
    #include "o2.hpp"
%}
