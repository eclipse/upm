// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_wheelencoder
%include "../upm.i"

%include "wheelencoder.hpp"
%{
    #include "wheelencoder.hpp"
%}
