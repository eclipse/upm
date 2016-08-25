// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_grovegsr
%include "../upm.i"

%feature("autodoc", "3");

%include "grovegsr.hpp"
%{
    #include "grovegsr.hpp"
%}
