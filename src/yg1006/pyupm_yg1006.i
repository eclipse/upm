// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_yg1006
%include "../upm.i"

%feature("autodoc", "3");

%include "yg1006.hpp"
%{
    #include "yg1006.hpp"
%}
