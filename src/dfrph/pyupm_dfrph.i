// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_dfrph
%include "../upm.i"

%include "dfrph.hpp"
%{
    #include "dfrph.hpp"
%}
