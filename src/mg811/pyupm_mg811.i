// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_mg811
%include "../upm.i"

%include "mg811.hpp"
%{
    #include "mg811.hpp"
%}
