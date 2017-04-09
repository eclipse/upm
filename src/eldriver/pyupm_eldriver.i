// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_eldriver
%include "../upm.i"

%feature("autodoc", "3");

%include "eldriver.hpp"
%{
    #include "eldriver.hpp"
%}
