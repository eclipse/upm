// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_groveeldriver
%include "../upm.i"

%feature("autodoc", "3");

%include "groveeldriver.hpp"
%{
    #include "groveeldriver.hpp"
%}
