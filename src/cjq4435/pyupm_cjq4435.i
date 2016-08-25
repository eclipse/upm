// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_cjq4435
%include "../upm.i"

%feature("autodoc", "3");

%include "cjq4435.hpp"
%{
    #include "cjq4435.hpp"
%}
