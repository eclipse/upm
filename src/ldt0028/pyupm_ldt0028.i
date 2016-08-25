// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_ldt0028
%include "../upm.i"

%feature("autodoc", "3");

%include "ldt0028.hpp"
%{
    #include "ldt0028.hpp"
%}

