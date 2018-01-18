// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_adc121c021
%include "../upm.i"

%include "adc121c021.hpp"
%{
    #include "adc121c021.hpp"
%}
