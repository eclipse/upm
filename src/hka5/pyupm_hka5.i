// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_hka5
%include "../upm.i"
%include "std_string.i"

%feature("autodoc", "3");

%include "hka5.hpp"
%{
    #include "hka5.hpp"
%}
