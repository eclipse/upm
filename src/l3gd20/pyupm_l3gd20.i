%module pyupm_l3gd20
// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"

%include "../upm.i"

%feature("autodoc", "3");

%include "l3gd20.hpp"
%{
    #include "l3gd20.hpp"
%}
