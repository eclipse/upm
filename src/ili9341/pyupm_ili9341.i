// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_ili9341
%include "../upm.i"

%include "ili9341_gfx.hpp"
%{
    #include "ili9341_gfx.hpp"
%}

%include "ili9341.hpp"
%{
    #include "ili9341.hpp"
%}
