// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_ehr
%include "../upm.i"

%feature("autodoc", "3");

%include "ehr.hpp"
%{
    #include "ehr.hpp"
%}
