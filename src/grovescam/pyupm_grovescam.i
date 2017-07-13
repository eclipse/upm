// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_grovescam
%include "../upm.i"

%{
    #include "grovescam.hpp"
%}
%include "grovescam.hpp"
