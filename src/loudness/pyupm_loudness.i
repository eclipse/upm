// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_loudness
%include "../upm.i"

%feature("autodoc", "3");

%include "loudness.hpp"
%{
    #include "loudness.hpp"
%}
