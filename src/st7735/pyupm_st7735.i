// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_st7735
%include "../upm.i"
%include "../carrays_uint8_t.i"

%include "st7735_gfx.hpp"
%{
    #include "st7735_gfx.hpp"
%}

%include "st7735.hpp"
%{
    #include "st7735.hpp"
%}
