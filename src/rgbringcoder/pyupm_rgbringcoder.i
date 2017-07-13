// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_rgbringcoder
%include "../upm.i"

%include "rgbringcoder.hpp"
%{
    #include "rgbringcoder.hpp"
%}
