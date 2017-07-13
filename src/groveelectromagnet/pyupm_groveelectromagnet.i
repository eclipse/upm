// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_groveelectromagnet
%include "../upm.i"

%include "groveelectromagnet.hpp"
%{
    #include "groveelectromagnet.hpp"
%}
