// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_a110x
%include "../upm.i"

%feature("autodoc", "3");

%include "a110x.hpp"
%{
    #include "a110x.hpp"
%}
