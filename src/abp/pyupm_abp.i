// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_abp
%include "../upm.i"

%include "abp.hpp"
%{
    #include "abp.hpp"
%}
