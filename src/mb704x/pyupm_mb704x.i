// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_mb704x
%include "../upm.i"
%include "std_string.i"

%include "mb704x.hpp"
%{
    #include "mb704x.hpp"
%}
