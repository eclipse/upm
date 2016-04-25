// Include doxygen-generated documentation
%include "pyupm_doxy2swig.i"
%module pyupm_ili9341
%include "../upm.i"

%feature("autodoc", "3");
%rename("printString") print(std::string msg);

%include "ili9341_gfx.hpp"
%{
    #include "ili9341_gfx.hpp"
%}

%include "ili9341.hpp"
%{
    #include "ili9341.hpp"
%}
