// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_groveeldriver
%include "../upm.i"

%include "groveeldriver.hpp"
%{
    #include "groveeldriver.hpp"
%}
