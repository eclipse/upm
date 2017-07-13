// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_eldriver
%include "../upm.i"

%include "eldriver.hpp"
%{
    #include "eldriver.hpp"
%}
