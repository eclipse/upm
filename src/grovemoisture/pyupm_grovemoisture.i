// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_grovemoisture
%include "../upm.i"

%include "grovemoisture.hpp"
%{
    #include "grovemoisture.hpp"
%}
