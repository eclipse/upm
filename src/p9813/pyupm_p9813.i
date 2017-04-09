// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_p9813
%include "../upm.i"

%feature("autodoc", "3");

%include "p9813.hpp"
%{
    #include "p9813.hpp"
%}
