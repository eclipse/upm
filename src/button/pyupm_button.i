// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_button
%include "../upm.i"

%feature("autodoc", "3");

%include "button.hpp"
%{
    #include "button.hpp"
%}
