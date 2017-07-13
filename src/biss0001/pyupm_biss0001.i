// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_biss0001
%include "../upm.i"

%include "biss0001.hpp"
%{
    #include "biss0001.hpp"
%}
