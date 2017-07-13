// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_light
%include "../upm.i"

%include "light.hpp"
%{
    #include "light.hpp"
%}
