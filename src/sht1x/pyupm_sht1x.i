// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_sht1x
%include "../upm.i"
%include "std_string.i"

%feature("autodoc", "3");

%include "sht1x.hpp"
%{
    #include "sht1x.hpp"
%}
