// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_mma8x5x
%include "../upm.i"

%include "mma8x5x.hpp"
%{
    #include "mma8x5x.hpp"
%}
