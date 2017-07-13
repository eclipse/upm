// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_vcap
%include "../upm.i"
%include "std_string.i"

%include "vcap.hpp"
%{
    #include "vcap.hpp"
%}


