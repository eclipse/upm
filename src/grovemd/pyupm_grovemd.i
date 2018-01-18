// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_grovemd
%include "../upm.i"

%include "grovemd.hpp"
%{
    #include "grovemd.hpp"
%}
