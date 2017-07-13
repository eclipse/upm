// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_groveo2
%include "../upm.i"

%include "groveo2.hpp"
%{
    #include "groveo2.hpp"
%}
