// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_grovespeaker
%include "../upm.i"

%feature("autodoc", "3");

%include "grovespeaker.hpp"
%{
    #include "grovespeaker.hpp"
%}
