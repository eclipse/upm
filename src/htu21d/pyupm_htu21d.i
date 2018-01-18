// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_htu21d
%include "../upm.i"

%include "htu21d.hpp"
%{
    #include "htu21d.hpp"
%}
