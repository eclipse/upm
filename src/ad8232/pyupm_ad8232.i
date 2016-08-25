// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_ad8232
%include "../upm.i"

%feature("autodoc", "3");

%include "ad8232.hpp"
%{
    #include "ad8232.hpp"
%}
