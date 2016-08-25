// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_lol
%include "../upm.i"

%include "stdint.i"

%feature("autodoc", "3");

%include "lol.hpp"
%{
    #include "lol.hpp"
%}
