// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_st7735
%include "../upm.i"

%feature("autodoc", "3");
%rename("printString") print(std::string msg);

%include "st7735_gfx.hpp"
%{
    #include "st7735_gfx.hpp"
%}

%include "st7735.hpp"
%{
    #include "st7735.hpp"
%}
