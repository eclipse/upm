// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_groveehr
%include "../upm.i"

%include "groveehr.hpp"
%{
    #include "groveehr.hpp"
%}
