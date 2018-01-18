// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_mag3110
%include "../upm.i"

%include "mag3110.hpp"
%{
    #include "mag3110.hpp"
%}
