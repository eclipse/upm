// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_adxrs610
%include "../upm.i"

%feature("autodoc", "3");

%include "adxrs610.hpp"
%{
    #include "adxrs610.hpp"
%}
