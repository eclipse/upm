// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_mma7455
%include "../upm.i"

%include "mma7455.hpp"
%{
    #include "mma7455.hpp"
%}
