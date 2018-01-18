// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_tsl2561
%include "../upm.i"

%include "tsl2561.hpp"
%{
    #include "tsl2561.hpp"
%}
