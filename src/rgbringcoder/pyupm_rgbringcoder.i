// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_rgbringcoder
%include "../upm.i"

%feature("autodoc", "3");

%include "rgbringcoder.h"
%{
    #include "rgbringcoder.h"
%}
