%module pyupm_rgbringcoder
%include "../upm.i"

%feature("autodoc", "3");

%include "rgbringcoder.h"
%{
    #include "rgbringcoder.h"
%}
