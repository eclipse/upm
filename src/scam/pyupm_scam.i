// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_scam
%include "../upm.i"

%{
    #include "scam.hpp"
%}
%include "scam.hpp"
