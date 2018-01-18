// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_si114x
%include "../upm.i"

%include "si114x.hpp"
%{
    #include "si114x.hpp"
%}
