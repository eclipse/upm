// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_max31723
%include "../upm.i"

%feature("autodoc", "3");

%include "max31723.hpp"
%{
    #include "max31723.hpp"
%}
