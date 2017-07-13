// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_loudness
%include "../upm.i"

%include "loudness.hpp"
%{
    #include "loudness.hpp"
%}
