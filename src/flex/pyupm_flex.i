// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_flex
%include "../upm.i"

%include "flex.hpp"
%{
    #include "flex.hpp"
%}
