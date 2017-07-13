// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_waterlevel
%include "../upm.i"

%include "waterlevel.hpp"
%{
    #include "waterlevel.hpp"
%}
