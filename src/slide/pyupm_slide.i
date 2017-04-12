// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_slide
%include "../upm.i"

%feature("autodoc", "3");

%include "slide.hpp"
%{
    #include "slide.hpp"
%}
