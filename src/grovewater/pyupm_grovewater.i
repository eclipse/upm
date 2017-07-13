// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_grovewater
%include "../upm.i"

%include "grovewater.hpp"
%{
    #include "grovewater.hpp"
%}
