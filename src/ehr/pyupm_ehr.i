// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_ehr
%include "../upm.i"

%include "ehr.hpp"
%{
    #include "ehr.hpp"
%}
