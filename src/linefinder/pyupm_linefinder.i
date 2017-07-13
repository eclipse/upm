// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_linefinder
%include "../upm.i"

%include "linefinder.hpp"
%{
    #include "linefinder.hpp"
%}
