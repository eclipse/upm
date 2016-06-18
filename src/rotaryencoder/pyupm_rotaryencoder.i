// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_rotaryencoder
%include "../upm.i"

%feature("autodoc", "3");

%include "rotaryencoder.hpp"
%{
    #include "rotaryencoder.hpp"
%}
