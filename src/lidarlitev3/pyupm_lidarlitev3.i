// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_lidarlitev3
%include "../upm.i"

%include "stdint.i"

%feature("autodoc", "3");

%include "lidarlitev3.hpp"
%{
    #include "lidarlitev3.hpp"
%}
