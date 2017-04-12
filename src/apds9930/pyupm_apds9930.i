%module pyupm_apds9930
%include "../upm.i"
// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"

%feature("autodoc", "3");

%include "apds9930.hpp"
%{
    #include "apds9930.hpp"
%}
