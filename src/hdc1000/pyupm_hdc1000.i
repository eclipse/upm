// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_hdc1000
%include "../upm.i"

%include "hdc1000.hpp"
%{
    #include "hdc1000.hpp"
%}
