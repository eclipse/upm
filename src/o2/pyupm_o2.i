// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_o2
%include "../upm.i"

%include "o2.hpp"
%{
    #include "o2.hpp"
%}
