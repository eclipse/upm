// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_rsc
%include "../upm.i"

%include "rsc.hpp"
%{
    #include "rsc.hpp"
%}
