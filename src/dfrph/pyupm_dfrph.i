// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_dfrph
%include "../upm.i"

%feature("autodoc", "3");

%include "dfrph.hpp"
%{
    #include "dfrph.hpp"
%}
