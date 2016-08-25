// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_l298
%include "../upm.i"

%feature("autodoc", "3");

%include "l298.hpp"
%{
    #include "l298.hpp"
%}
