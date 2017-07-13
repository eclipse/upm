// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_grovecollision
%include "../upm.i"

%include "grovecollision.hpp"
%{
    #include "grovecollision.hpp"
%}
